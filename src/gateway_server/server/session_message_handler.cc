//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 11:37:16.
// File name: session_message_handler.cc
//
// Description:
//

#include "gateway_server/server/session_message_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/gateway_user.h"
#include "global/logging.h"
#include "session_server/client/session_client.h"
#include "session_server/protocol/session_gateway_protocol_types.h"

namespace gateway {

namespace server {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  session::client::SessionClientPtr session =
    GatewayServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Session client is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  session->SetMessageHandler(session::protocol::MessageType::MESSAGE_GATEWAY_SERVER,
      boost::bind(&SessionMessageHandler::OnMessage, this, _1, _2));
  session->SetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD,
      boost::bind(&SessionMessageHandler::OnMessageForward, this, _1, _2));
  session->SetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_BROADCAST,
      boost::bind(&SessionMessageHandler::OnMessageBroadcast, this, _1, _2));
  session->SetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST,
      boost::bind(&SessionMessageHandler::OnMessageMulticast, this, _1, _2));
  return true;
}

void SessionMessageHandler::Finalize() {
  session::client::SessionClientPtr session =
    GatewayServerSingleton::GetInstance().GetSessionClient();
  if(session.get() == NULL) {
    global::LogError("%s:%d (%s) Session client is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  session->ResetMessageHandler(session::protocol::MessageType::MESSAGE_GATEWAY_SERVER);
  session->ResetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
  session->ResetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_BROADCAST);
  session->ResetMessageHandler(session::protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
}

void SessionMessageHandler::OnMessage(const char *message, size_t size) {
  session::protocol::MessageGatewayServer gate_message;
  if(this->thrift_.Deserialize(gate_message, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(gate_message.type_) {
    case session::protocol::GatewayMessageType::MESSAGE_USER_LOGIN_RESPONSE:
      this->OnActorLoginResponse(gate_message.message_.data(),
          gate_message.message_.size());
      break;
    case session::protocol::GatewayMessageType::MESSAGE_KICKOUT_USER:
      this->OnKickoutActor(gate_message.message_.data(),
          gate_message.message_.size());
      break;
    case session::protocol::GatewayMessageType::MESSAGE_USER_SWITCH_GAME_REQUEST:
      this->OnActorSwitchGame(gate_message.message_.data(),
          gate_message.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]",
          __FILE__, __LINE__, __FUNCTION__, gate_message.type_);
      break;
  }
}

void SessionMessageHandler::OnMessageForward(const char *message, size_t size) {
  // Deserialize message.
  session::protocol::MessageUserMessageForward forward;
  if(this->thrift_.Deserialize(forward, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Get user.
  GatewayUser *user = GatewayServerSingleton::GetInstance().GetUser(forward.actor_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Forward message failed, user[%lld] not exist.",
        __FILE__, __LINE__, __FUNCTION__, forward.actor_);
    return ;
  }
  // Send message to user.
  protocol::Message message2;
  message2.__set_message_(forward.message_);
  message2.__set_type_((gateway::protocol::MessageType::type)forward.type_);
  user->SendMessage(message2);
}

void SessionMessageHandler::OnMessageBroadcast(const char *message, size_t size) {
  // Deserialize message.
  session::protocol::MessageUserMessageBroadcast broadcast;
  if(this->thrift_.Deserialize(broadcast, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // Get users.
  GatewayServer::LoginUserMap &users = GatewayServerSingleton::GetInstance().GetUsers();
  GatewayServer::LoginUserMap::iterator iterator = users.begin();
  for(; iterator != users.end(); ++iterator) {
    if(iterator->second) {
      // Send message to user.
      protocol::Message message2;
      message2.__set_message_(broadcast.message_);
      message2.__set_type_((gateway::protocol::MessageType::type)broadcast.type_);
      iterator->second->SendMessage(message2);
    }
  }
}

void SessionMessageHandler::OnMessageMulticast(const char *message, size_t size) {
  // Deserialize message.
  session::protocol::MessageUserMessageMulticast multicast;
  if(this->thrift_.Deserialize(multicast, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  std::vector<int64_t>::const_iterator iterator = multicast.actors_.begin();
  for(; iterator != multicast.actors_.end(); ++iterator) {
    // Get user.
    GatewayUser *user = GatewayServerSingleton::GetInstance().GetUser(*iterator);
    if(user == NULL) {
      return ;
    }
    // Send message to user.
    protocol::Message message2;
    message2.__set_message_(multicast.message_);
    message2.__set_type_((gateway::protocol::MessageType::type)multicast.type_);
    user->SendMessage(message2);
  }
}

void SessionMessageHandler::OnActorLoginResponse(const char *message, size_t size) {
  session::protocol::GateMessageUserLoginResponse response;
  if(this->thrift_.Deserialize(response, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  GatewayUser *user = GatewayServerSingleton::GetInstance().GetUser(response.actor_id_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Get user failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  user->SetGameServer(response.game_server_);
  // 发登陆结果事件给玩家
  StatusEventUserLogin event;
  event.result_ = response.logined_;
  user->OnEvent(StatusEventType::EVENT_USER_LOGIN, (const char *)&event, sizeof(event));
}

void SessionMessageHandler::OnKickoutActor(const char *message, size_t size) {
  session::protocol::GateMessageKickoutUser kickout;
  if(this->thrift_.Deserialize(kickout, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 得到待踢出玩家
  GatewayUser *user = GatewayServerSingleton::GetInstance().GetUser(kickout.actor_id_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Get user failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 发送踢出消息给玩家
  protocol::MessageKickoutActor message2;
  message2.__set_type_(kickout.type_);
  user->SendMessage(message2, protocol::MessageType::MESSAGE_KICKOUT_ACTOR);

  global::LogDebug("[踢出玩家流程] 网关踢出玩家(%llu)，原因(%d)",
      user->GetActorID().GetID(), kickout.type_);

  user->Exit();
}

void SessionMessageHandler::OnActorSwitchGame(const char *message, size_t size) {
    session::protocol::GateMessageUserSwitchGameRequest request;
  if(this->thrift_.Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  GatewayUser *user = GatewayServerSingleton::GetInstance().GetUser(request.actor_);
  if(user == NULL) {
    global::LogError("%s:%d (%s) Get user failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  user->SetGameServer(request.game_server_);
}

}  // namespace server

}  // namespace gateway

