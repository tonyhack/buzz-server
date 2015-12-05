//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-08 14:52:46.
// File name: actor_attr_event_handler.cc
//
// Description:
// Define class ActorAttrEventHandler.
//

#include "game_server/server/extension/soul/actor_attr_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

ActorAttrEventHandler::ActorAttrEventHandler() {}
ActorAttrEventHandler::~ActorAttrEventHandler() {}

bool ActorAttrEventHandler::Initialize() {
  this->conn_change_current_energy_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorAttrEventHandler::OnEventRoleChangeCurrentEnergy, this, _1, _2, _3, _4));

  this->conn_change_max_energy_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MAX_ENERGY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorAttrEventHandler::OnEventRoleChangeMaxEnergy, this, _1, _2, _3, _4));

  this->conn_common_status_change_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
        &ActorAttrEventHandler::OnEventRoleCommonStatusChange, this, _1, _2, _3, _4));

  return true;
}

void ActorAttrEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_current_energy_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_change_max_energy_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_common_status_change_);
}

void ActorAttrEventHandler::OnEventRoleChangeCurrentEnergy(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventRoleChangeCurrentEnergy) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleChangeCurrentEnergy *event =
    (event::EventRoleChangeCurrentEnergy *)message;

  SoulActor *actor = SoulActorManager::GetInstance()->Get(event->id_);
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return;
  }

  if (actor->CheckLoadFinish() == true) {
    GameActor *game_actor = actor->GetActor();
    if (game_actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
      return;
    }

    SoulEnergy *energy = actor->GetSoulEnergy();
    if (energy == NULL) {
      LOG_ERROR("获取 SoulEnergy(%lu) 失败", event->id_);
      return;
    }

    EnergyStatus *current_status = energy->GetCurrentStatus();
    if (current_status == NULL) {
      LOG_ERROR("获取玩家(%lu) EnergyStatus 失败");
      return;
    }

    current_status->OnEvent(event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY,
        message, size);
  }
}

void ActorAttrEventHandler::OnEventRoleChangeMaxEnergy(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventRoleChangeMaxEnergy) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleChangeMaxEnergy *event =
    (event::EventRoleChangeMaxEnergy *)message;

  SoulActor *actor = SoulActorManager::GetInstance()->Get(event->id_);
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return;
  }

  if (actor->CheckLoadFinish() == true) {
    GameActor *game_actor = actor->GetActor();
    if (game_actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
      return;
    }

    SoulEnergy *energy = actor->GetSoulEnergy();
    if (energy == NULL) {
      LOG_ERROR("获取 SoulEnergy(%lu) 失败", event->id_);
      return;
    }

    EnergyStatus *current_status = energy->GetCurrentStatus();
    if (current_status == NULL) {
      LOG_ERROR("获取玩家(%lu) EnergyStatus 失败");
      return;
    }

    current_status->OnEvent(event::EventType::EVENT_ROLE_CHANGE_MAX_ENERGY,
        message, size);
  }
}

void ActorAttrEventHandler::OnEventRoleCommonStatusChange(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventRoleCommonStatusChange) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleCommonStatusChange *event =
    (event::EventRoleCommonStatusChange *)message;

  SoulActor *actor = SoulActorManager::GetInstance()->Get(event->id_);
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return;
  }

  if (actor->CheckLoadFinish() == true) {
    GameActor *game_actor = actor->GetActor();
    if (game_actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
      return;
    }

    SoulEnergy *energy = actor->GetSoulEnergy();
    if (energy == NULL) {
      LOG_ERROR("获取 SoulEnergy(%lu) 失败", event->id_);
      return;
    }

    EnergyStatus *current_status = energy->GetCurrentStatus();
    if (current_status == NULL) {
      LOG_ERROR("获取玩家(%lu) EnergyStatus 失败");
      return;
    }

    current_status->OnEvent(event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE,
        message, size);
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

