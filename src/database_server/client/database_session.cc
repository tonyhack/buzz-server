//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 11:08:13.
// File name: database_session.cc
//
// Description:
// Define class DatabaseSession.
//

#include "database_server/client/database_session.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"

namespace database {

namespace client {

DatabaseSession::DatabaseSession() : packet_(NULL), status_(STATUS_NOT_LOGIN) {}

DatabaseSession::~DatabaseSession() {}

bool DatabaseSession::Initialize(const core::InetAddress &server_address,
    const std::string &name, core::EventLoop *network_loop, core::EventLoop *application_loop,
    const global::protocol::ServerSignature &signature, global::ThriftPacket *packet) {
  assert(network_loop && application_loop && packet);
  this->packet_ = packet;
  this->SetServerSignature(signature);
  if(this->client_.Initialize(server_address, name, network_loop, application_loop,
        boost::bind(&DatabaseSession::OnConnection, this->shared_from_this(), _1)) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize client failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void DatabaseSession::Finalize() {
  this->client_.Finalize();
  this->packet_ = NULL;
  if(this->connection_.get() != NULL) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_->Finalize();
  }
}

void DatabaseSession::Start() {
  this->SetStatus(STATUS_NOT_LOGIN);
  this->client_.Start();
}

void DatabaseSession::Stop() {
  this->SetStatus(STATUS_STOP);
  this->client_.Stop();
}

void DatabaseSession::SendMessage(const protocol::Message &message) {
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

void DatabaseSession::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  }
}

void DatabaseSession::OnConnection(const coresh::ConnectionPtr &connection) {
  assert(connection);
  this->connection_ = connection;
  this->connection_->SetDisconnectCallback(
      boost::bind(&DatabaseSession::OnDisconnect, this->shared_from_this()));
  this->connection_->SetMessageCallback(
      boost::bind(&DatabaseSession::OnMessage, this->shared_from_this(), _1, _2));

  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Connect to database server [host=%s] completely.",
      this->client_.GetServerAddress().ToString().c_str());

  // Send login request message.
  protocol::MessageLoginRequest message;
  message.signature_ = this->GetServerSignature();
  this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST);
}

void DatabaseSession::OnMessage(const char *message, size_t size) {
  assert(this->packet_);
  protocol::Message message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    switch(message2.type_) {
      case protocol::MessageType::MESSAGE_LOGIN_RESPONSE:
        this->OnMessageLoginResponse(message2.message_.data(),
            message2.message_.size());
        break;
      case protocol::MessageType::MESSAGE_LOGOUT_RESPONSE:
        this->OnMessageLogoutResponse(message2.message_.data(),
            message2.message_.size());
        break;
      case protocol::MessageType::MESSAGE_STORAGE_RESPONSE:
        this->OnMessageStorageResponse(message2.message_.data(),
            message2.message_.size());
        break;
      default:
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Error message type[%d]", __FILE__, __LINE__,
            __FUNCTION__, message2.type_);
        break;
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseSession::OnDisconnect() {
  this->connection_->ResetDisconnectCallback();
  this->connection_->ResetMessageCallback();
  this->connection_.reset();
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "Disconnected database server [type=%d, id=%d, host=%s:%d], reconnect it.",
      this->GetServerSignature().type_, this->GetServerSignature().id_,
      this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);

  if(this->GetStatus() != STATUS_STOP) {
    this->client_.Start();
  } else {
    DatabaseClientSingleton::GetInstance().RemoveSession(this->GetServerSignature().id_);
  }
}

void DatabaseSession::OnMessageLoginResponse(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLoginResponse message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "Login database server [host=%s] successful.",
        this->client_.GetServerAddress().ToString().c_str());
    if(message2.logined_ == true) {
      this->SetStatus(STATUS_LOGINED);
      DatabaseClientSingleton::GetInstance().OnDatabaseSessionLogined(
          this->GetServerSignature());
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Login database server[host=%s] failed.",
          __FILE__, __LINE__, __FUNCTION__,
          this->client_.GetServerAddress().ToString().c_str());
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseSession::OnMessageLogoutResponse(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageLogoutResponse message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    if(message2.logouted_) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
          "Logout database server [host=%s] successful.",
          this->client_.GetServerAddress().ToString().c_str());
      this->SetStatus(STATUS_NOT_LOGIN);
    } else {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Logout database server [host-%s] failed.",
          __FILE__, __LINE__, __FUNCTION__,
          this->client_.GetServerAddress().ToString().c_str());
    }
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

void DatabaseSession::OnMessageStorageResponse(const char *message, size_t size) {
  assert(this->packet_);
  protocol::MessageStorageResponse message2;
  if(this->packet_->Deserialize(message2, message, size)) {
    DatabaseClientSingleton::GetInstance().ReceiveResponse(message2);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
  }
}

}  // client

}  // database

