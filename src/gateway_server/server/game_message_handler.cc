//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 10:35:45.
// File name: game_message_handler.cc
//
// Description:
// Define class GameMessagehandler.
//

#include "gateway_server/server/game_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/client/game_client.h"
#include "game_server/client/game_session.h"
#include "game_server/protocol/game_protocol_types.h"
#include "gateway_server/server/gateway_server.h"
#include "gateway_server/server/gateway_user.h"
#include "global/logging.h"

namespace gateway {

namespace server {

GameMessagehandler::GameMessagehandler() {}
GameMessagehandler::~GameMessagehandler() {}

bool GameMessagehandler::Initialize() {
  game::client::GameClientSingleton::GetInstance().SetMessageHandler(
      game::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD,
      boost::bind(&GameMessagehandler::OnUserMessageForward, this, _1, _2, _3));
  game::client::GameClientSingleton::GetInstance().SetMessageHandler(
      game::protocol::MessageType::MESSAGE_USER_MESSAGE_BROADCAST,
      boost::bind(&GameMessagehandler::OnUserMessageBroadcast, this, _1, _2, _3));
  game::client::GameClientSingleton::GetInstance().SetMessageHandler(
      game::protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST,
      boost::bind(&GameMessagehandler::OnUserMessageMulticast, this, _1, _2, _3));
  return true;
}

void GameMessagehandler::Finalize() {
  game::client::GameClientSingleton::GetInstance().ResetMessageHandler(
      game::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
}

void GameMessagehandler::OnMessage(const game::client::GameSessionPtr &session,
    const char *message, size_t size) {
  game::protocol::Message game_message;
  if(this->thrift_.Deserialize(game_message, message, size) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(game_message.type_) {
    case game::protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD:
      this->OnUserMessageForward(session, game_message.message_.data(),
          game_message.message_.size());
      break;
    case game::protocol::MessageType::MESSAGE_USER_MESSAGE_BROADCAST:
      this->OnUserMessageBroadcast(session, game_message.message_.data(),
          game_message.message_.size());
      break;
    case game::protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST:
      this->OnUserMessageMulticast(session, game_message.message_.data(),
          game_message.message_.size());
      break;
    default:
      global::LogError("%s:%d (%s) Error message type[%d]", game_message.type_);
      break;
  }
}

void GameMessagehandler::OnUserMessageForward(const game::client::GameSessionPtr &session,
    const char *message, size_t size) {
  // Deserialize message.
  game::protocol::MessageUserMessageForward forward;
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

void GameMessagehandler::OnUserMessageBroadcast(const game::client::GameSessionPtr &session,
    const char *message, size_t size) {
  // Deserialize message.
  game::protocol::MessageUserMessageBroadcast broadcast;
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

void GameMessagehandler::OnUserMessageMulticast(const game::client::GameSessionPtr &session,
    const char *message, size_t size) {
  // Deserialize message.
  game::protocol::MessageUserMessageMulticast multicast;
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

}  // namespace server

}  // namespace gateway

