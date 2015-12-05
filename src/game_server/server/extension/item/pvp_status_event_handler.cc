//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-21 16:50:18.
// File name: pvp_status_event_handler.cc
//
// Description:
// Define PvpStatusEventHandler.
//

#include "game_server/server/extension/item/pvp_status_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

PvpStatusEventHandler::PvpStatusEventHandler() {}
PvpStatusEventHandler::~PvpStatusEventHandler() {}

bool PvpStatusEventHandler::Initialize() {
  this->conn_enter_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_ENTER_PVP_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&PvpStatusEventHandler::OnEventEnterPvpStatus,
        this, _1, _2, _3, _4));

  this->conn_enter_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_LEAVE_PVP_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&PvpStatusEventHandler::OnEventLeavePvpStatus,
        this, _1, _2, _3, _4));

  return true;
}

void PvpStatusEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_enter_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_leave_);
}

void PvpStatusEventHandler::OnEventEnterPvpStatus(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleEnterPvpStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleEnterPvpStatus *event = (event::EventRoleEnterPvpStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(event->id_);
  if(item_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->id_);
    return ;
  }

  item_actor->SetPvpStatus(true);
}

void PvpStatusEventHandler::OnEventLeavePvpStatus(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleLeavePvpStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleLeavePvpStatus *event = (event::EventRoleLeavePvpStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(event->id_);
  if(item_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->id_);
    return ;
  }

  item_actor->SetPvpStatus(false);
}

}  // namespace item

}  // namespace server

}  // namespace game

