//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-09 19:58:02.
// File name: actor_die_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/bar/actor_die_event_handler.h"

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
#include "game_server/server/event/game_role_event_types.h"

namespace game {

namespace server {

namespace bar {

ActorDieEventHandler::ActorDieEventHandler() {}
ActorDieEventHandler::~ActorDieEventHandler() {}

bool ActorDieEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_die_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_ROLE_KILLED,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorDieEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorDieEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_die_);
}

void ActorDieEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleKilled) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;
  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    LOG_DEBUG("杀死的不是玩家");
    return ;
  }

  // 得到进度条玩家对象
  BarActor *actor = BarActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  if(actor->BarExist() == true) {
    // 取消进度条
    actor->CancelBar(entity::BarEventType::ACTOR_DIE);
  }
}

}  // namespace bar

}  // namespace server

}  // namespace game
