//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-09 17:43:50.
// File name: schedule_terminal.cc
//
// Description:
// Define class ScheduleTerminal.
//

#include "schedule_server/server/schedule_terminal.h"

#include <boost/bind.hpp>

#include "core/async_logging_singleton.h"
#include "schedule_server/server/schedule_server.h"

namespace schedule {

namespace server {

ScheduleTerminal::ScheduleTerminal() : packet_(NULL) {}
ScheduleTerminal::~ScheduleTerminal() {}

bool ScheduleTerminal::Initialize(const coresh::ConnectionPtr &connection,
    global::ThriftPacket *packet) {
  assert(connection);
  assert(packet);
  this->connection_ = connection;
  this->packet_ = packet;

  this->connection_->SetDisconnectCallback(
      boost::bind(&ScheduleTerminal::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&ScheduleTerminal::OnMessage, this, _1, _2));
  
  return true;
}

void ScheduleTerminal::Finalize() {
  if(this->connection_) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
    this->packet_ = NULL;
  }
}

void ScheduleTerminal::SendMessage(const protocol::Message &message) {
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

void ScheduleTerminal::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  }
}

void ScheduleTerminal::OnDisconnect() {
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Schedule terminal [%d,%d] disconnected.", this->GetSignature().type_,
      this->GetSignature().id_);
  ScheduleServerSingleton::GetInstance().OnDisconnect(this);
}

void ScheduleTerminal::OnMessage(const char *message, size_t size) {
  assert(this->packet_);
  protocol::Message message1;
  if(this->packet_->Deserialize(message1, message, size)) {
    switch(message1.type_) {
      case protocol::MessageType::MESSAGE_LOGIN_REQUEST:
        this->OnMessageLogin(message1.message_.data(), message1.message_.size());
        break;
      case protocol::MessageType::MESSAGE_LOGOUT_REQUEST:
        this->OnMessageLogout(message1.message_.data(), message1.message_.size());
        break;
      case protocol::MessageType::MESSAGE_GATEWAY_USER_NUMBER_UPLOAD:
        this->OnMessageUploadGatewayUserNumber(message1.message_.data(),
            message1.message_.size());
        break;
      default:
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message type[%d]", __FILE__, __LINE__, __FUNCTION__,
            message1.type_);
        break;
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleTerminal::OnMessageLogin(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLoginRequest request;
  if(this->packet_->Deserialize(request, message, size)) {
    this->SetSignature(request.signature_);
    if(ScheduleServerSingleton::GetInstance().RequestLogin(this)) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "Schedule terminal type=%d id=%d ip=%s port=%d logined",
          this->GetSignature().type_, this->GetSignature().id_,
          this->GetSignature().host_.c_str(), this->GetSignature().port_);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "Schedule terminal type=%d id=%d ip=%s port=%d login failed",
          request.signature_.type_, request.signature_.id_,
          request.signature_.host_.c_str(), request.signature_.port_);
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void ScheduleTerminal::OnMessageLogout(const char *message, size_t size) {
  if(ScheduleServerSingleton::GetInstance().RequestLogout(this)) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Schedule terminal type=%d id=%d ip=%s port=%d logouted",
        this->GetSignature().type_, this->GetSignature().id_,
        this->GetSignature().host_.c_str(), this->GetSignature().port_);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "Schedule terminal type=%d id=%d ip=%s port=%d logout failed",
        this->GetSignature().type_, this->GetSignature().id_,
        this->GetSignature().host_.c_str(), this->GetSignature().port_);
  }
}

void ScheduleTerminal::OnMessageUploadGatewayUserNumber(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageGatewayUserNumberUpload message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    protocol::MessageGatewayUserNumberBroadcast broadcast;
    broadcast.__set_server_id_(this->GetSignature().id_);
    broadcast.__set_number_(message2.number_);
    ScheduleServerSingleton::GetInstance().BroadcastMessage(broadcast,
        protocol::MessageType::MESSAGE_GATEWAY_USER_NUMBER_BROADCAST);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // namespace server

}  // namespace schedule

