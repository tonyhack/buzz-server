//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 15:04:43.
// File name: status_verify_question_handler.cc
//
// Description:
// Define class StatusVerifyQuestionHandler.
//

#include "gateway_server/server/status_verify_question_handler.h"

#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/gateway_user.h"
#include "global/server_configure.h"
#include "global/simple_encrypt.h"

namespace gateway {

namespace server {

StatusVerifyQuestionHandler::StatusVerifyQuestionHandler() : random_code_(0) {}
StatusVerifyQuestionHandler::~StatusVerifyQuestionHandler() {}

bool StatusVerifyQuestionHandler::Initialize(GatewayUser *user) {
  if(StatusHandler::Initialize(user) == false) {
    global::LogError("%s:%d (%s) StatusHandler initialize failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Generate random key.
  global::SimpleEncrypt::MakeRandomKey(this->random_key_, sizeof(this->random_key_));

  return true;
}

void StatusVerifyQuestionHandler::Finalize() {
  StatusHandler::Finalize();
  this->random_code_ = 0;
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_VERIFY_QUESTION_TIMEOUT, this);
}

UserStatus::Type StatusVerifyQuestionHandler::GetStatus() {
  return UserStatus::VERIFY_QUESTION_STATUS;
}

void StatusVerifyQuestionHandler::Start() {
  GatewayServerSingleton::GetInstance().AddTimer(TIMER_VERIFY_QUESTION_TIMEOUT,
      VERIFY_QUESTION_TIMEOUT_SECS * 1000, 1, this, "LoginStatusHandler::Start");
  this->SendQuestion();
}

void StatusVerifyQuestionHandler::End() {
  GatewayServerSingleton::GetInstance().RemoveTimer(TIMER_VERIFY_QUESTION_TIMEOUT, this);
}

void StatusVerifyQuestionHandler::GotoNextStatus() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->user_->GotoStatus(UserStatus::CHECK_SESSION_STATUS);
}

void StatusVerifyQuestionHandler::OnMessage(const char *message, size_t size) {
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Get thrift packet failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  if(thrift->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(message2.type_) {
    case protocol::MessageType::MESSAGE_VERIFY_QUESTION_REQUEST:
      this->OnMessageQuestionRequest(message2.message_.data(), message2.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message2.type_);
      break;
  }
}

void StatusVerifyQuestionHandler::OnTimer(core::uint32 id) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(id == TIMER_VERIFY_QUESTION_TIMEOUT) {
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
    global::LogDebug("Gateway user[%lld] verify question timeout!",
        this->user_->GetActorID().GetID());
  }
}

void StatusVerifyQuestionHandler::SendQuestion() {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->random_code_ =
    GatewayServerSingleton::GetInstance().GetRandomGenerator()->Random() % 100;
  protocol::MessageVerifyQuestion question;
  question.__set_code_(this->random_code_);
  question.key_.assign(this->random_key_, sizeof(this->random_key_));

  this->user_->SendMessage(question, protocol::MessageType::MESSAGE_VERIFY_QUESTION);
}

void StatusVerifyQuestionHandler::OnMessageQuestionRequest(
    const char *message, size_t size) {
  if(this->user_ == NULL) {
    global::LogError("%s:%d (%s) Gateway user pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  assert(thrift);
  // Deserialize message.
  protocol::MessageVerifyQuestionRequest request;
  if(thrift->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Check question.
  char buffer[512] = {0};
  int buffer_size = 512;
  global::SimpleEncrypt::Encrypt(
      global::ServerConfigureSingleton().GetInstance().GetQuestion().c_str(),
      this->random_key_, this->random_code_, buffer, buffer_size);
  buffer[buffer_size] = 0;
  if(strncmp(request.value_.c_str(), buffer, sizeof(buffer)) != 0) {
    this->user_->GotoStatus(UserStatus::LOGOUT_STATUS);
  } else {
    protocol::MessageVerifyQuestionResponse response;
    response.__set_pass_(true);
    this->user_->SendMessage(response,
        protocol::MessageType::MESSAGE_VERIFY_QUESTION_RESPONSE);
    this->GotoNextStatus();
  }
}

}  // namespace server

}  // namespace gateway

