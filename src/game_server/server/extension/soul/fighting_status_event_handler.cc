//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-21 17:16:58.
// File name: fighting_status_event_handler.cc
//
// Description:
// Define class FightingStatusEventHandler.
//

#include "game_server/server/extension/soul/fighting_status_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

FightingStatusEventHandler::FightingStatusEventHandler() {}
FightingStatusEventHandler::~FightingStatusEventHandler() {}

bool FightingStatusEventHandler::Initialize() {
  this->conn_enter_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_ENTER_FIGHTING_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&FightingStatusEventHandler::OnEventEnterFightingStatus,
        this, _1, _2, _3, _4));

  this->conn_leave_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_LEAVE_FIGHTING_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&FightingStatusEventHandler::OnEventLeaveFightingStatus,
        this, _1, _2, _3, _4));

  return true;
}

void FightingStatusEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_enter_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_leave_);
}

void FightingStatusEventHandler::OnEventEnterFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleEnterFightingStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleEnterFightingStatus *event =
    (event::EventRoleEnterFightingStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->id_);
  if(soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return ;
  }

  soul_actor->SetFightingStatus(true);
}

void FightingStatusEventHandler::OnEventLeaveFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleLeaveFightingStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleLeaveFightingStatus *event =
    (event::EventRoleLeaveFightingStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->id_);
  if(soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return ;
  }

  soul_actor->SetFightingStatus(false);
}

}  // namespace soul

}  // namespace server

}  // namespace game

