//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-31 17:45:27.
// File name: gateway_user.cc
//
// Description:
// Define class GatewayUser.
//

#include "gateway_server/server/gateway_user.h"

#include <boost/bind.hpp>

namespace gateway {

namespace server {

GatewayUser::GatewayUser() : logined_(false), status_(NULL) {}
GatewayUser::~GatewayUser() {}

bool GatewayUser::Initialize(const coresh::ConnectionPtr &connection) {
  assert(connection);
  this->connection_ = connection;

  this->SetLogined(false);

  // Initialize status handlers.
  if(this->verify_question_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize verify question status failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->check_session_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize check session status failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->login_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize login status failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->running_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize running status failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->logout_status_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize logout status failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connection_->SetDisconnectCallback(
      boost::bind(&GatewayUser::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&GatewayUser::OnMessage, this, _1, _2));

  // Start verify question Status.
  this->status_ = &this->verify_question_status_;
  this->status_->Start();

  return true;
}

void GatewayUser::Finalize() {
  this->SetLogined(false);
  this->status_ = NULL;

  this->logout_status_.Finalize();
  this->running_status_.Finalize();
  this->login_status_.Finalize();
  this->check_session_status_.Finalize();
  this->verify_question_status_.Finalize();

  if(this->connection_) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
  }
}

void GatewayUser::SendMessage(const protocol::Message &message) {
  global::ThriftPacket *thrift = GatewayServerSingleton::GetInstance().GetThrift();
  assert(thrift);
  size_t size = 0;
  const char *message1 = thrift->Serialize(message, size);
  if(message1) {
    this->SendMessage(message1, size);
  } else {
    global::LogError("%s:%d (%s) Serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void GatewayUser::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  }
}
/*
void GatewayUser::FinishCheckSession(bool pass) {
  if(this->status_ == NULL) {
    global::LogError("%s:%d (%s) Status pointer is null",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(this->status_->GetStatus() != UserStatus::CHECK_SESSION_STATUS) {
    global::LogWarning("%s:%d (%s) Error user status",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->check_session_status_.FinishCheckSession(pass);
}

void GatewayUser::FinishUserLogin(bool pass) {
  if(this->status_ == NULL) {
    global::LogError("%s:%d (%s) Status pointer is null",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(this->status_->GetStatus() != UserStatus::LOGIN_STATUS) {
    global::LogWarning("%s:%d (%s) Error user status",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->login_status_.FinishUserLogin(pass);
}
*/
void GatewayUser::OnEvent(StatusEventType::type type, const char *event, size_t size) {
  if(this->status_ == NULL) {
    global::LogError("%s:%d (%s) Status pointer is null",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->status_->OnEvent(type, event, size);
}

void GatewayUser::GotoStatus(UserStatus::Type type) {
  if(this->status_ == NULL) {
    global::LogError("%s:%d (%s) Status pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->status_->End();
  switch(type) {
    case UserStatus::VERIFY_QUESTION_STATUS:
      this->status_ = &this->verify_question_status_;
      break;
    case UserStatus::CHECK_SESSION_STATUS:
      this->status_ = &this->check_session_status_;
      break;
    case UserStatus::LOGIN_STATUS:
      this->status_ = &this->login_status_;
      break;
    case UserStatus::RUNNING_STATUS:
      this->status_ = &this->running_status_;
      break;
    case UserStatus::LOGOUT_STATUS:
      this->status_ = &this->logout_status_;
      break;
    default:
      this->status_ = NULL;
      global::LogError("%s:%d (%s) Error status[%d]",
          type, __FILE__, __LINE__, __FUNCTION__);
      break;
  }
  if(this->status_) {
    this->status_->Start();
  }
}

void GatewayUser::Exit() {
  this->connection_->ApplicationDisconnect();
}

void GatewayUser::OnDisconnect() {
  if(this->status_) {
    this->status_->OnDisconnect();
  }
  GatewayServerSingleton::GetInstance().Logout(this);
  GatewayServerSingleton::GetInstance().OnDisconnect(this);
}

void GatewayUser::OnMessage(const char *message, size_t size) {
  if(this->status_ == NULL) {
    global::LogError("%s:%d (%s) Status pointer is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->status_->OnMessage(message, size);
}

}  // namespace server

}  // namespace gateway

