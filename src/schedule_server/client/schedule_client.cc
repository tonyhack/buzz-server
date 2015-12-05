//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-10 17:20:33.
// File name: schedule_client.cc
//
// Description:
// Define class ScheduleClient.
//

#include "schedule_server/client/schedule_client.h"

#include <vector>

#include <boost/bind.hpp>

namespace schedule {

namespace client {

ScheduleClient::ScheduleClient() : packet_(NULL) {}

ScheduleClient::~ScheduleClient() {}

bool ScheduleClient::Initialize(const core::InetAddress &server_address,
    const std::string &name, core::EventLoop *network_loop, core::EventLoop *application_loop,
    const global::protocol::ServerSignature &signature, global::ThriftPacket *packet) {
  assert(network_loop && application_loop && packet);

  this->packet_ = packet;
  this->SetServerSignature(signature);
  if(this->client_.Initialize(server_address, name, network_loop, application_loop,
        boost::bind(&ScheduleClient::OnConnection, this, _1)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize client failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Set Message handler.
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_RESPONSE,
      boost::bind(&ScheduleClient::OnMessageLoginResponse, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGIN_BROADCAST,
      boost::bind(&ScheduleClient::OnMessageLoginBroadcast, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_RESPONSE,
      boost::bind(&ScheduleClient::OnMessageLogoutResponse, this, _1, _2));
  this->SetMessageHandler(protocol::MessageType::MESSAGE_LOGOUT_BROADCAST,
      boost::bind(&ScheduleClient::OnMessageLogoutBroadcast, this, _1, _2));

  return true;
}

void ScheduleClient::Finalize() {
  this->client_.Finalize();
  if(this->connection_.get() != NULL) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_->Finalize();
  }
}

void ScheduleClient::Start() {
  this->client_.Start();
}

void ScheduleClient::Stop() {
  this->client_.Stop();
}

void ScheduleClient::SendMessage(const protocol::Message &message) {
  assert(this->packet_);
  size_t size = 0;
  const char *message1 = this->packet_->Serialize(message, size);
  if(message1) {
    this->SendMessage(message1, size);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  } 
}

void ScheduleClient::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  } 
}

void ScheduleClient::OnConnection(const coresh::ConnectionPtr &connection) {
  assert(connection);
  this->connection_ = connection;
  this->connection_->SetDisconnectCallback(
      boost::bind(&ScheduleClient::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&ScheduleClient::OnMessage, this, _1, _2));

  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Connect schedule server [host=%s] completely.",
      this->client_.GetServerAddress().ToString().c_str());

  // Send login request message.
  protocol::MessageLoginRequest message;
  message.signature_ = this->GetServerSignature();
  this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST);
}

void ScheduleClient::OnMessage(const char *message, size_t size) {
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

void ScheduleClient::OnDisconnect() {
  this->connection_->ResetDisconnectCallback();
  this->connection_->ResetMessageCallback();
  this->connection_.reset();
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Disconnected schedule server [type=%d, id=%d, host=%s:%d], reconnect it.",
      this->GetServerSignature().type_, this->GetServerSignature().id_,
      this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);
  this->client_.Start();
}

void ScheduleClient::OnMessageLoginResponse(const char *message, size_t size) {
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

void ScheduleClient::OnMessageLogoutResponse(const char *message, size_t size) {
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

void ScheduleClient::OnMessageLoginBroadcast(const char *message, size_t size) {
  if(this->join_callback_) {
    assert(this->packet_);
    protocol::MessageLoginBroadcast message2;
    if(this->packet_->Deserialize(message2, message, size)) {
      this->join_callback_(message2.signature_);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

void ScheduleClient::OnMessageLogoutBroadcast(const char *message, size_t size) {
  if(this->disjoin_callback_) {
    assert(this->packet_);
    protocol::MessageLogoutBroadcast message2;
    if(this->packet_->Deserialize(message2, message, size)) {
      global::protocol::ServerSignature signature;
      signature.__set_type_(message2.type_);
      signature.__set_id_(message2.id_);
      this->disjoin_callback_(signature);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
    }
  }
}

}  // namespace client

}  // namespace schedule

