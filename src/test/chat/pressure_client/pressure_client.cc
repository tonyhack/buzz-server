//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-06 15:48:26.
// File name: pressure_client.cc
//
// Description:
// Define class PressureClient.
//

#include "test/chat/pressure_client/pressure_client.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "coresh/packetizer.h"
#include "coresh/packetizer_table.h"
#include "coresh/protocol_format.h"
#include "coresh/variable_table.h"
#include "test/chat/pressure_client/configure.h"
#include "test/chat/pressure_client/multiplex_thrift.h"
#include "test/chat/pressure_client/pressure_client_set.h"
#include "test/chat/protocol/protocol_types.h"

namespace test {

namespace chat {

namespace pressure {

const core::uint32 kMaxChatFrequencyPlusMilisecond = 100;

PressureClient::PressureClient(const core::InetAddress &server_address,
    const std::string &name) : client_(server_address, name),
  name_(name), login_status_(false), thrift_(NULL) {}

PressureClient::~PressureClient() {}

bool PressureClient::Allocate() {
  return true;
}

void PressureClient::Deallocate() {}

bool PressureClient::Initialize(core::EventLoop *loop) {
  assert(loop);
  if(this->client_.Initialize(loop) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize tcp client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->thrift_ = MultiplexThriftSinglton::GetInstance().GetThriftPacket(loop);
  if(this->thrift_ == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    this->Stop();
    return false;
  }

  this->client_.SetConnectionCallback(boost::bind(&PressureClient::OnConnection, this, _1));
  this->client_.SetMessageCallback(boost::bind(&PressureClient::OnMessage, this, _1, _2, _3));
  this->client_.SetWriteCallback(boost::bind(&PressureClient::OnWriteComplete, this, _1));

  return true;
}

void PressureClient::Finalize() {
  this->SetLoginStatus(false);
  this->thrift_ = NULL;
}

void PressureClient::Start() {
  this->client_.Start();
}

void PressureClient::Stop() {
  this->client_.Stop();
}

template <typename MessageType>
void PressureClient::SendMessage(const MessageType &message,
    protocol::MessageType::type type) {
  size_t size = 0;
  const char *serialize = this->thrift_->Serialize(message, size);
  if(serialize) {
    protocol::Message msg;
    msg.__set_type_(type);
    msg.message_.assign(serialize, size);
    msg.__isset.message_ = true;
    serialize = this->thrift_->Serialize(msg, size);
    if(serialize) {
      this->SendMessage(serialize, size);
      return ;
    }
  }
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
      "%s:%d (%s) Serialize message failed", __FILE__, __LINE__, __FUNCTION__);
}

void PressureClient::OnTimer(core::uint32 id) {
  if(id == TIMER_TYPE_CHAT) {
    this->Chat();
  }
}

void PressureClient::OnConnection(const core::TCPConnectionPtr &connection) {
  if(connection->CheckConnected()) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Pressure client[%s] connected.", this->GetName().c_str());
    usleep(1000);
    protocol::MessageLoginRequest message;
    message.__set_name_(this->GetName());
    this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Pressure client[%s] disconnected.", this->GetName().c_str());
    this->client_.GetEventLoop()->RemoveTimer(TIMER_TYPE_CHAT, this);
  }
}

void PressureClient::OnMessage(const core::TCPConnectionPtr &connection,
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

void PressureClient::OnWriteComplete(const core::TCPConnectionPtr &connection) {
}

void PressureClient::SendMessage(const char *message, size_t size) {
  coresh::Packetizer *packetizer = coresh::PacketizerTableSingleton::GetInstance().GetPacketizer(
      this->client_.GetEventLoop());
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
        "%s:%d (%s) Get resusable buffer or Packetizer failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void PressureClient::OnMessage(const char *message, size_t size) {
  protocol::Message msg;
  if(this->thrift_->Deserialize(msg, message, size)){
    switch(msg.type_) {
      case protocol::MessageType::MESSAGE_LOGIN_RESPONSE:
        this->OnMessageLoginResponse(msg.message_.data(), msg.message_.size());
        break;
      case protocol::MessageType::MESSAGE_LOGIN_BROADCAST:
        break;
      case protocol::MessageType::MESSAGE_LOGOUT_BROADCAST:
        break;
      case protocol::MessageType::MESSAGE_CHAT:
        break;
      default:
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message type[%d]",
            __FILE__, __LINE__, __FUNCTION__, (int)msg.type_);
        break;
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void PressureClient::OnMessageLoginResponse(const char *message, size_t size) {
  protocol::MessageLoginResponse msg;
  if(this->thrift_->Deserialize(msg, message, size)) {
    if(msg.success_) {
      this->SetLoginStatus(true);
      core::uint32 chat_frequency_plus = PressureClientSetSingleton::GetInstance().GetRandomGenerator().Random();
      chat_frequency_plus %= ConfigureSingleton::GetInstance().chat_frequency_ + kMaxChatFrequencyPlusMilisecond;

      this->client_.GetEventLoop()->AddTimer(TIMER_TYPE_CHAT, ConfigureSingleton::GetInstance().chat_frequency_ + chat_frequency_plus,
          TimerHandler::INFINITE_COUNT, this, "PressureClient::Initialize");

    } else {
      this->SetLoginStatus(false);
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Login server failed",
          __FILE__, __LINE__, __FUNCTION__);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void PressureClient::Chat() {
  if(this->GetLoginStatus()) {
    core::uint32 random = PressureClientSetSingleton::GetInstance().GetRandomGenerator().Random();
    protocol::MessageChat message;
    message.__set_type_(protocol::ChatType::CHAT_BROADCAST);
    message.__set_from_(this->GetName());
    message.__set_contect_(ConfigureSingleton::GetInstance().GetChat(random));
    this->SendMessage(message, protocol::MessageType::MESSAGE_CHAT);
  }
}

}  // namespace pressure

}  // namespace chat

}  // namespace test

