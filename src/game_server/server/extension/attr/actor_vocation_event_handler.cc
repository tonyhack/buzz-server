//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-24 22:16:37.
// File name: actor_vocation_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/attr/actor_vocation_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"

namespace game {

namespace server {

namespace attr {

ActorVocationEventHandler::ActorVocationEventHandler() {}
ActorVocationEventHandler::~ActorVocationEventHandler() {}

bool ActorVocationEventHandler::Initialize() {
  // Follow 所有玩家保存事件
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_VOCATION, 
      coresh::Communicator::GetChannelAll(),event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorVocationEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorVocationEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_);
}

void ActorVocationEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventRoleChangeVocation) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventRoleChangeVocation *event = (event::EventRoleChangeVocation *)message;

  LOG_DEBUG("玩家(%lu) ATTR扩展保存");

  AttrActor *actor = AttrActorManager::GetInstance()->Get(event->id_);
  if (NULL == actor) {
    LOG_ERROR("AttrActor(%lu) does not found in AttrActorManager.", event->id_);
    return;
  }
  actor->GetRuneActor()->OnVocation(event->src_vocation_);
}

}  // namespace attr

}  // namespace server

}  // namespace game

