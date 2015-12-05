//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-14 16:26:06.
// File name: schedule_client2.cc
//
// Description:
// Define class ScheduleClient2.
//

#include "schedule_server/client/schedule_client2.h"

#include <vector>

#include <boost/bind.hpp>

#include "coresh/packetizer_table.h"
#include "coresh/protocol_format.h"
#include "coresh/variable_table.h"

namespace schedule {

namespace client {

ScheduleClient2::ScheduleClient2() : packet_(NULL) {}

ScheduleClient2::~ScheduleClient2() {}

bool ScheduleClient2::Initialize(const core::InetAddress &server_address, const std::string &name,
    core::EventLoop *loop, const global::protocol::ServerSignature &signature,
    global::ThriftPacket *packet) {
  assert(loop);
  assert(packet);

  if(this->client_.Initialize(server_address, name, loop) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize tcp client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->packet_ = packet;
  this->SetServerSignature(signature);

  this->client_.SetConnectionCallback(
      boost::bind(&ScheduleClient2::OnConnection, this, _1));
  this->client_.SetMessageCallback(
      boost::bind(&ScheduleClient2::OnMessage, this, _1, _2, _3));
  this->client_.SetWriteCallback(
      boost::bind(&ScheduleClient2::OnWriteComplete, this, _1));

  // Set Message handler.
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_RESPONSE,
      boost::bind(&ScheduleClient2::OnMessageLoginResponse, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_BROADCAST,
      boost::bind(&ScheduleClient2::OnMessageLoginBroadcast, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_RESPONSE,
      boost::bind(&ScheduleClient2::OnMessageLogoutResponse, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_BROADCAST,
      boost::bind(&ScheduleClient2::OnMessageLogoutBroadcast, this, _1, _2));

  return true;
}

void ScheduleClient2::Finalize() {
  this->client_.Finalize();
  this->packet_ = NULL;
}

void ScheduleClient2::Start() {
  this->client_.Start();
}

void ScheduleClient2::Stop() {
  this->client_.Stop();
}

void ScheduleClient2::SendMessage(const protocol::Message &message,
    global::ThriftPacket *packet) {
  assert(packet);
  size_t size = 0;
  const char *message1 = packet->Serialize(message, size);
  if(message1) {
    this->SendMessage(message1, size);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::SendMessage(const char *message, size_t size) {
  core::EventLoop *loop = this->client_.GetEventLoop();
  assert(loop && loop->CheckInLoopThread());
  coresh::Packetizer *packetizer =
    coresh::PacketizerTableSingleton::GetInstance().GetPacketizer(loop);
  if(packetizer) {
    coresh::ProtocolHead head = 0;
    size_t packet_size = 0;
    int retcode = 0;
    retcode = packetizer->Packet(message, size);
    if(retcode > 0) {
      const char *packet = packetizer->GetPacketMessage(packet_size);
      head = packet_size;
      coresh::SetProtocolEncode(head, packetizer->GetEncodeType());
      coresh::SetProtocolCompress(head, packetizer->GetCompressType());
      this->client_.SendMessage((const char *)&head, sizeof(head));
      this->client_.SendMessage(packet, packet_size);
    } else if(retcode == 0) {
      head = size;
      coresh::SetProtocolEncode(head, 0);
      coresh::SetProtocolCompress(head, 0);
      this->client_.SendMessage((const char *)&head, sizeof(head));
      this->client_.SendMessage(message, size);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Packet failed", __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get Packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnConnection(const core::TCPConnectionPtr &connection) {
  assert(connection);
  if(connection->CheckConnected()) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Connect schedule server [host=%s] completely.",
        this->client_.GetServerAddress().ToString().c_str());
    // Send login request message.
    protocol::MessageLoginRequest message;
    message.signature_ = this->GetServerSignature();
    this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST, this->packet_);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Disconnected schedule server [type=%d, id=%d, host=%s:%d].",
        this->GetServerSignature().type_, this->GetServerSignature().id_,
        this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);
    this->client_.Start();
  }
}

void ScheduleClient2::OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick) {
  coresh::Packetizer *packetizer = coresh::PacketizerTableSingleton::GetInstance().GetPacketizer(
      this->client_.GetEventLoop());
  if(packetizer) {
    size_t read_size = 0, message_size = 0, unpacket_size = 0;
    int retcode = 0;
    const coresh::ProtocolHead *protocol_head = NULL;
    while(true) {
      read_size = buffer.ReadSize();
      // Can read a message head.
      if(read_size < coresh::kProtocolHeadSize) {
        break;
      }
      message_size = coresh::GetProtocolSize(buffer.ReadPointer());
      // Can read a whole message.
      if(message_size > coresh::VariableTableSingleton::GetInstance().GetMaxProtocolSize()) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message size[%llu] in protocol head.",
            __FILE__, __LINE__, __FUNCTION__, message_size);
        connection->Shutdown();
        break;
      } else if(read_size < message_size) {
        break;
      }
      protocol_head = coresh::ProtocolHeadCast(buffer.ReadPointer());
      if(coresh::GetProtocolEncode(*protocol_head) != (core::uint32)packetizer->GetEncodeType() ||
          coresh::GetProtocolCompress(*protocol_head) != (core::uint32)packetizer->GetCompressType()) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error encode/compress type in protocol head.",
            __FILE__, __LINE__, __FUNCTION__);
        connection->Shutdown();
        break;
      }
      retcode = packetizer->Unpacket(coresh::GetProtocolData(buffer.ReadPointer()), message_size);
      if(retcode > 0) {
        const char *message = packetizer->GetUnpacketMessage(unpacket_size);
        this->OnMessage(message, unpacket_size);
      } else if(retcode == 0) {
        this->OnMessage(coresh::GetProtocolData(buffer.ReadPointer()),
            message_size - coresh::kProtocolHeadSize);
      } else {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Unpacket message failed and will be ignored",
            __FILE__, __LINE__, __FUNCTION__);
      }
      buffer.ReadShift(message_size);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnWriteComplete(const core::TCPConnectionPtr &connection) {}

void ScheduleClient2::OnMessage(const char *message, size_t size) {
  assert(this->packet_);
  protocol::Message message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(message2.type_ >= protocol::MessageType::MESSAGE_MIN &&
        message2.type_ < protocol::MessageType::MESSAGE_MAX &&
        message_handlers_[message2.type_]) {
      this->message_handlers_[message2.type_](message2.message_.data(),
          message2.message_.size());
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Error message type[%d]", __FILE__, __LINE__,
          __FUNCTION__, message2.type_);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnMessageLoginResponse(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLoginResponse message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(message2.logined_) {
      std::vector<global::protocol::ServerSignature> &signatures = message2.servers_;
      std::vector<global::protocol::ServerSignature>::iterator iterator = signatures.begin();
      for(; iterator != signatures.end(); ++iterator) {
        if(this->join_callback_) {
          this->join_callback_(*iterator);
        }
      }
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "Login schedule server successful.");
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Login schedule server failed.", __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnMessageLogoutResponse(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLogoutResponse message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(message2.logouted_) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "Logout schedule server successful.");
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Logout schedule server failed.", __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnMessageLoginBroadcast(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLoginBroadcast message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(this->join_callback_) {
      this->join_callback_(message2.signature_);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleClient2::OnMessageLogoutBroadcast(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLogoutBroadcast message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(this->disjoin_callback_) {
      global::protocol::ServerSignature signature;
      signature.__set_type_(message2.type_);
      signature.__set_id_(message2.id_);
      this->disjoin_callback_(signature);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace client

}  // namespace schedule

