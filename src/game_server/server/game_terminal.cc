//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-06 10:58:17.
// File name: game_terminal.cc
//
// Description:
// Define class GameTerminal.
//

#include "game_server/server/game_terminal.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"

namespace game {

namespace server {

GameTerminal::GameTerminal() : thrift_(NULL) {}
GameTerminal::~GameTerminal() {}

bool GameTerminal::Initialize(const coresh::ConnectionPtr &connection,
    global::ThriftPacket *packet) {
  if(connection.get() == NULL) {
    global::LogError("%s:%d (%s) Argument connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(packet == NULL) {
    global::LogError("%s:%d (%s) Argument connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connection_ = connection;
  this->thrift_ = packet;

  this->connection_->SetDisconnectCallback(
      boost::bind(&GameTerminal::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&GameTerminal::OnMessage, this, _1, _2));

  return true;
}

void GameTerminal::Finalize() {
  if(this->connection_) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
    this->thrift_ = NULL;
  }
}

void GameTerminal::SendMessage(const protocol::Message &message) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->SendMessage(message1, size);
}

void GameTerminal::SendMessage(const char *message, size_t size) {
  if(this->connection_.get() == NULL) {
    global::LogError("%s:%d (%s) Connection is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->connection_->SendMessage(message, size);
}

void GameTerminal::OnDisconnect() {
  global::LogInfo("Game terminal [%d,%d] disconnected.",
      this->GetSignature().type_, this->GetSignature().id_);
  GameServerSingleton::GetInstance().OnDisconnect(this);
}

void GameTerminal::OnMessage(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message1;
  if(this->thrift_->Deserialize(message1, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(message1.type_) {
    case protocol::MessageType::MESSAGE_LOGIN_REQUEST:
      this->OnMessageLogin(message1.message_.data(), message1.message_.size());
      break;
    case protocol::MessageType::MESSAGE_LOGOUT_REQUEST:
      this->OnMessageLogout(message1.message_.data(), message1.message_.size());
      break;
    case protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD:
      this->OnMessageForward(message1.message_.data(), message1.message_.size());
      break;
    case protocol::MessageType::MESSAGE_USER_LOGOUT:
      this->OnMessageActorLogout(message1.message_.data(), message1.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message1.type_);
  }
}

void GameTerminal::OnMessageLogin(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageLoginRequest request;
  if(this->thrift_->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  this->SetSignature(request.signature_);
  if(GameServerSingleton::GetInstance().OnTerminalLogin(this) == false) {
    global::LogError("%s:%d (%s) Terminal[type=%d, id=%d, ip=%s, port=%d] login failed.",
        __FILE__, __LINE__, __FUNCTION__, request.signature_.type_, request.signature_.id_,
        request.signature_.host_.c_str(), request.signature_.port_);
    return ;
  }
  global::LogInfo("Game termial type=%d id=%d ip=%s port=%d logined.",
      request.signature_.type_, request.signature_.id_, request.signature_.host_.c_str(),
      request.signature_.port_);
}

void GameTerminal::OnMessageLogout(const char *message, size_t size) {
  if(GameServerSingleton::GetInstance().OnTerminalLogout(this) == false) {
    global::LogError("%s:%d (%s) Terminal[type=%d, id=%d, ip=%s, port=%d] logout failed.",
        __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_, this->GetSignature().id_,
        this->GetSignature().host_.c_str(), this->GetSignature().port_);
    return ;
  }
  global::LogInfo("Session terminal type=%d id=%d ip=%s port=%d logouted.",
      this->GetSignature().type_, this->GetSignature().id_,
      this->GetSignature().host_.c_str(), this->GetSignature().port_);
}

void GameTerminal::OnMessageForward(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    global::LogError("%s:%d (%s) Thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageUserMessageForward forward;
  if(this->thrift_->Deserialize(forward, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  ExtensionManager::GetInstance()->OnActorMessage(
      (gateway::protocol::MessageType::type)forward.type_, forward.actor_,
      forward.message_.data(), forward.message_.size());
}

void GameTerminal::OnMessageActorLogout(const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 解析消息
  protocol::MessageUserLogout message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageUserLogout 失败");
    return ;
  }

  // 获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(message.actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor(%lu) 失败", message.actor_id_);
    return ;
  }
  GameActorManager::GetInstance()->KickActor(message.actor_id_);
}

}  // namespace server

}  // namespace game

