//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-06 14:17:35.
// File name: task_message_handler.cc
//
// Description:
// Define class ActorMessageHandler.
//

#include "game_server/server/actor_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

ActorMessageHandler::ActorMessageHandler() {}
ActorMessageHandler::~ActorMessageHandler() {}

bool ActorMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_FUNCTIONALITY_OPEN_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageActorFunctionalityOpenRequest, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_USE_TASLIMAN_REQUEST,
      boost::bind(&ActorMessageHandler::OnMessageActorUseTaslimanRequest, this, _1, _2, _3));

  return true;
}

void ActorMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_FUNCTIONALITY_OPEN_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_USE_TASLIMAN_REQUEST);

}

void ActorMessageHandler::OnMessageActorFunctionalityOpenRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageActorFunctionalityOpenRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LoadFunctionalityConfigure::OpenCells &cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::CLIENT_REQUEST);
  for(size_t i = 0; i < cells.size(); ++i) {
    if(cells[i].function_ == message.functionality_id_) {
      if(game_actor->CheckFunctionalityState(message.functionality_id_) == false) {
        game_actor->SetFunctionalityState(message.functionality_id_);
        gateway::protocol::MessageActorFunctionalityOpenResponse response;
        response.__set_functionality_id_(message.functionality_id_);
        game_actor->SendMessage(response,
            gateway::protocol::MessageType::MESSAGE_ACTOR_FUNCTIONALITY_OPEN_RESPONSE);
        return ;
      }
    }
  }
}

void ActorMessageHandler::OnMessageActorUseTaslimanRequest(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageActorUseTaslimanRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(message.type_ == gateway::protocol::UseTaslimanType::HP) {
    if(game_actor->UseHpTasliman() == false) {
      LOG_ERROR("使用护符失败");
      return ;
    }
  } else {
    if(game_actor->UseMpTasliman() == false) {
      LOG_ERROR("使用兰护符失败");
      return ;
    }
  }
  gateway::protocol::MessageActorUseTaslimanResponse response;
  response.__set_type_(message.type_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ACTOR_USE_TASLIMAN_RESPONSE);

}

}  // namespace server

}  // namespace game

