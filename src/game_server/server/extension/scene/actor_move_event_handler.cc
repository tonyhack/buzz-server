//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-08 00:21:53.
// File name: actor_move_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/scene/actor_move_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"

namespace game {

namespace server {

namespace scene {

ActorMoveEventHandler::ActorMoveEventHandler() {}
ActorMoveEventHandler::~ActorMoveEventHandler() {}

bool ActorMoveEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_move_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorMoveEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorMoveEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_move_);
}

void ActorMoveEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneRoleChangeLocation) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSceneRoleChangeLocation *event =
    (event::EventSceneRoleChangeLocation *)message;

  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("游戏玩家对象没有找到");
    return ;
  }
  if(actor->GetBasicStatus() == entity::RoleBasicStatus::STATUS_ZAZEN) {
    actor->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON);
  }

}

}  // namespace scene

}  // namespace server

}  // namespace game
