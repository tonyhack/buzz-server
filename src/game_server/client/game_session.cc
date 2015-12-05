//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-22 16:12:30.
// File name: game_session.cc
//
// Description:
//

#include "game_server/client/game_session.h"

#include <boost/bind.hpp>

#include "game_server/client/game_client.h"

namespace game {

namespace client {

GameSession::GameSession() : thrift_(NULL), status_(STATUS_NOT_LOGIN) {}

GameSession::~GameSession() {}

bool GameSession::Initialize(const core::InetAddress &server_address,
    const std::string &name, core::EventLoop *network_loop, core::EventLoop *app_loop,
    const global::protocol::ServerSignature &signature, global::ThriftPacket *thrift) {
  // Check argument.
  if(network_loop == NULL || app_loop == NULL || thrift == NULL) {
    global::LogError("%s:%d (%s) Argument network_loop/app_loop/thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->thrift_ = thrift;
  this->SetServerSignature(signature);
  // Initialize tcp client.
  if(this->client_.Initialize(server_address, name, network_loop, app_loop,
        boost::bind(&GameSession::OnConnection, this->shared_from_this(), _1)) == false) {
    global::LogError("%s:%d (%s) Initialize client failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void GameSession::Finalize() {
  this->client_.Finalize();
  this->thrift_ = NULL;
  if(this->connection_.get() != NULL) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_->Finalize();
  }
}

void GameSession::Start() {
  this->SetStatus(STATUS_NOT_LOGIN);
  this->client_.Start();
}

void GameSession::Stop() {
  this->SetStatus(STATUS_STOP);
  this->client_.Stop();
}

void GameSession::SendMessage(const protocol::Message &message) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) serialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->SendMessage(message1, size);
}

void GameSession::SendMessage(const char *message, size_t size) {
  if(this->connection_.get() == NULL) {
    global::LogError("%s:%d (%s) Connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_->SendMessage(message, size);
}

void GameSession::OnConnection(const coresh::ConnectionPtr &connection) {
  if(connection.get() == NULL) {
    global::LogError("%s:%d (%s) Argument connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_ = connection;
  this->connection_->SetDisconnectCallback(
      boost::bind(&GameSession::OnDisconnect, this->shared_from_this()));
  this->connection_->SetMessageCallback(
      boost::bind(&GameSession::OnMessage, this->shared_from_this(), _1, _2));

  global::LogInfo("Connect to game server [host=%s] completely",
      this->client_.GetServerAddress().ToString().c_str());

  // Send login request.
  protocol::MessageLoginRequest message;
  message.signature_ = GameClientSingleton::GetInstance().GetServerSignature();
  this->SendMessage(message, protocol::MessageType::MESSAGE_LOGIN_REQUEST);
}

void GameSession::OnMessage(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(message2.type_) {
    case protocol::MessageType::MESSAGE_LOGIN_RESPONSE:
      this->OnMessageLoginResponse(message2.message_.data(), message2.message_.size());
      break;
    case protocol::MessageType::MESSAGE_LOGOUT_RESPONSE:
      this->OnMessageLogoutResponse(message2.message_.data(), message2.message_.size());
      break;
    default:
      GameClientSingleton::GetInstance().OnMessage(message2.type_, this->shared_from_this(),
          message2.message_.data(), message2.message_.size());
      break;
  }
}

void GameSession::OnDisconnect() {
  if(this->connection_.get() == NULL) {
    global::LogError("%s:%d (%s) Connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_->ResetDisconnectCallback();
  this->connection_->ResetMessageCallback();
  this->connection_.reset();

  if(this->GetStatus() != STATUS_STOP) {
    global::LogInfo("Disconnected server [type=%d, id=%d, host=%s:%d], reconnect it.",
        this->GetServerSignature().type_, this->GetServerSignature().id_,
        this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);
    this->client_.Start();
  } else {
    global::LogInfo("Disconnected server [type=%d, id=%d, host=%s:%d].",
        this->GetServerSignature().type_, this->GetServerSignature().id_,
        this->GetServerSignature().host_.c_str(), this->GetServerSignature().port_);
    GameClientSingleton::GetInstance().RemoveSession(this->GetServerSignature().id_);
  }
}

void GameSession::OnMessageLoginResponse(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageLoginResponse message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.logined_ == false) {
    global::LogError("%s:%d (%s) Login game server[host=%s] failed.",
        __FILE__, __LINE__, __FUNCTION__,
        this->client_.GetServerAddress().ToString().c_str());
    return ;
  }
  global::LogInfo("Login game server [host=%s] successful.",
      this->client_.GetServerAddress().ToString().c_str());
  this->SetStatus(STATUS_LOGINED);
}

void GameSession::OnMessageLogoutResponse(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageLogoutResponse message2;
  if(this->thrift_->Deserialize(message2, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.logouted_ == false) {
    global::LogError("%s:%d (%s) Logout game server [host=%s] failed.",
        __FILE__, __LINE__, __FUNCTION__,
        this->client_.GetServerAddress().ToString().c_str());
    return ;
  }
  global::LogInfo("Logout game server [host=%s] successful.",
      this->client_.GetServerAddress().ToString().c_str());
  this->SetStatus(STATUS_NOT_LOGIN);
}

}  // namespace client

}  // namespace game

