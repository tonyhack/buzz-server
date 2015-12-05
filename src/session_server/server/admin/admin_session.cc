#include "session_server/server/admin/admin_session.h"

#include <boost/bind.hpp>

#include "session_server/server/admin/admin_service.h"
#include "session_server/server/admin/admin_session_manager.h"

namespace session {

namespace server {

namespace admin {

AdminSession::AdminSession() : session_id_(0) {}
AdminSession::~AdminSession() {}

bool AdminSession::Initialize(uint64_t session_id, const coresh::ConnectionPtr &connection) {
  this->session_id_ = session_id;
  this->connection_ = connection;

  this->connection_->SetDisconnectCallback(
      boost::bind(&AdminSessionManager::CloseSession,
          AdminSessionManager::GetInstance(), session_id));
  this->connection_->SetMessageCallback(
      boost::bind(&AdminSession::OnMessage, this, _1, _2));

  return true;
}

void AdminSession::Finalize() {
  if (this->connection_) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
  }
  this->session_id_ = 0;
}

void AdminSession::Close() {
  this->connection_->ApplicationDisconnect();
}

void AdminSession::OnMessage(const char *message, size_t size) {
  AdminService::GetInstance()->OnMessage(this, message, size);
}

}  // namespace admin

}  // namespace server

}  // namespace session

