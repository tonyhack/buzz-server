//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-04 10:37:54.
// File name: session_terminal.cc
//
// Description:
// Define class SessionTerminal.
//

#include "session_server/server/session_terminal.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/game_message_handler.h"
#include "session_server/server/gateway_message_handler.h"
#include "session_server/server/session_actor_pool.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"

namespace session {

namespace server {

SessionTerminal::SessionTerminal() : thrift_(NULL) {}
SessionTerminal::~SessionTerminal() {}

bool SessionTerminal::Initialize(const coresh::ConnectionPtr &connection,
    global::ThriftPacket *packet) {
  // connection and thrift.
  assert(connection);
  assert(packet);
  this->connection_ = connection;
  this->thrift_ = packet;

  // Connection callbacks.
  this->connection_->SetDisconnectCallback(
      boost::bind(&SessionTerminal::OnDisconnect, this));
  this->connection_->SetMessageCallback(
      boost::bind(&SessionTerminal::OnMessage, this, _1, _2));

  return true;
}

void SessionTerminal::Finalize() {
  if(this->connection_) {
    this->connection_->ResetDisconnectCallback();
    this->connection_->ResetMessageCallback();
    this->connection_.reset();
    this->thrift_ = NULL;
  }
}

void SessionTerminal::SendMessage(const protocol::Message &message) {
  assert(this->thrift_);
  size_t size = 0;
  const char *message1 = this->thrift_->Serialize(message, size);
  if(message1) {
    this->SendMessage(message1, size);
  } else {
    global::LogError("%s:%d (%s) Deserialize message failed",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void SessionTerminal::SendMessage(const char *message, size_t size) {
  if(this->connection_) {
    this->connection_->SendMessage(message, size);
  }
}

void SessionTerminal::OnDisconnect() {
  global::LogInfo("Session terminal [%d,%d] disconnected.",
      this->GetSignature().type_, this->GetSignature().id_);

  // 发送登出事件
  event::EventSystemServerLogout event;
  event.__set_type_(this->GetSignature().type_);
  event.__set_id_(this->GetSignature().id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SYSTEM_SERVER_LOGOUT, 0,
      event::ChannelType::CHANNEL_SYSTEM, &event, sizeof(event));

  SessionServerSingleton::GetInstance().OnDisconnect(this);
}

void SessionTerminal::OnMessage(const char *message, size_t size) {
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
    case protocol::MessageType::MESSAGE_GAME_SERVER:
      this->OnMessageGame(message1.message_.data(), message1.message_.size());
      break;
    case protocol::MessageType::MESSAGE_GATEWAY_SERVER:
      this->OnMessageGateway(message1.message_.data(), message1.message_.size());
      break;
    case protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD:
      this->OnMessageForward(message1.message_.data(), message1.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, message1.type_);
  }
}

void SessionTerminal::OnMessageLogin(const char *message, size_t size) {
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
  if(SessionServerSingleton::GetInstance().RequestLogin(this) == false) {
    global::LogError("%s:%d (%s) Terminal[type=%d, id=%d, ip=%s, port=%d] login failed.",
        __FILE__, __LINE__, __FUNCTION__, request.signature_.type_,
        request.signature_.id_, request.signature_.host_.c_str(),
        request.signature_.port_);
    return ;
  }
  global::LogInfo("Session termial type=%d id=%d ip=%s port=%d logined.",
      request.signature_.type_, request.signature_.id_,
      request.signature_.host_.c_str(), request.signature_.port_);

  // 发送登出事件
  event::EventSystemServerLogin event;
  event.__set_type_(this->GetSignature().type_);
  event.__set_id_(this->GetSignature().id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_SYSTEM_SERVER_LOGIN, 0,
      event::ChannelType::CHANNEL_SYSTEM, &event, sizeof(event));
}

void SessionTerminal::OnMessageLogout(const char *message, size_t size) {
  if(SessionServerSingleton::GetInstance().RequestLogout(this)) {
    global::LogInfo("Session terminal type=%d id=%d ip=%s port=%d logouted.",
        this->GetSignature().type_, this->GetSignature().id_,
        this->GetSignature().host_.c_str(), this->GetSignature().port_);
  } else {
    global::LogError("%s:%d (%s) Terminal type=%d id=%d ip=%s port=%d logout failed.",
        __FILE__, __LINE__, __FUNCTION__, this->GetSignature().type_,
        this->GetSignature().id_, this->GetSignature().host_.c_str(),
        this->GetSignature().port_);
  }
}

void SessionTerminal::OnMessageGame(const char *message, size_t size) {
  if(this->GetSignature().type_ != global::protocol::ServerType::GAME_SERVER) {
    global::LogError("%s:%d (%s) Terminal is not game server.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  GameMessageHandler::GetInstance()->OnMessage(this, message, size);
}

void SessionTerminal::OnMessageGateway(const char *message, size_t size) {
  if(this->GetSignature().type_ != global::protocol::ServerType::GATEWAY_SERVER) {
    global::LogError("%s:%d (%s) Terminal is not gateway server.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  GatewayMessageHandler::GetInstance()->OnMessage(this, message, size);
}

void SessionTerminal::OnMessageForward(const char *message, size_t size) {
  if(this->thrift_ == NULL) {
    LOG_ERROR("Thrift packet is null.");
    return ;
  }
  protocol::MessageUserMessageForward forward;
  if(this->thrift_->Deserialize(forward, message, size) == false) {
    LOG_ERROR("Deserialize message failed.");
    return ;
  }
  ExtensionManager::GetInstance()->OnActorMessage(
      (gateway::protocol::MessageType::type)forward.type_, forward.actor_,
      forward.message_.data(), forward.message_.size());
}

}  // namespace server

}  // namespace session

