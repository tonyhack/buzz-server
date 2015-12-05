//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 14:59:43.
// File name: status_logout_handler.cc
//
// Description:
// Define class StatusLogoutHandler.
//


#include "gateway_server/server/status_login_handler.h"

#include "gateway_server/server/gateway_user.h"

namespace gateway {

namespace server {

StatusLogoutHandler::StatusLogoutHandler() {}
StatusLogoutHandler::~StatusLogoutHandler() {}

bool StatusLogoutHandler::Initialize(GatewayUser *user) {
  if(StatusHandler::Initialize(user) == false) {
    global::LogError("%s:%d (%s) StatusHandler initialize failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void StatusLogoutHandler::Finalize() {
  StatusHandler::Finalize();
}

UserStatus::Type StatusLogoutHandler::GetStatus() {
  return UserStatus::LOGOUT_STATUS;
}

void StatusLogoutHandler::Start() {
  if(this->user_)
    this->user_->Exit();
}

void StatusLogoutHandler::End() {
}

void StatusLogoutHandler::GotoNextStatus() {
}

void StatusLogoutHandler::OnMessage(const char *message, size_t size) {}

}  // namespace server

}  // namespace gateway

