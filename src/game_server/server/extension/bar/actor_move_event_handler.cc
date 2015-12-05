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

#include "game_server/server/extension/bar/actor_move_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/bar_types.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/bar/bar_actor.h"
#include "game_server/server/extension/bar/bar_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"

namespace game {

namespace server {

namespace bar {

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

  // 得到进度条玩家对象
  BarActor *actor = BarActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }
  
  if(actor->BarExist() == false) {
    return ;
  }

  // 取消进度条
  actor->CancelBar(entity::BarEventType::ACTOR_MOVE);
}

}  // namespace bar

}  // namespace server

}  // namespace game
