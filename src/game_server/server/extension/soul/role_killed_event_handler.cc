//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-11 10:17:57.
// File name: role_killed_event_handler.cc
//
// Description:
// Define class RoleKilledEventHandler.
//

#include "game_server/server/extension/soul/role_killed_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_energy.h"
#include "game_server/server/extension/soul/soul_energy_status.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace soul {

RoleKilledEventHandler::RoleKilledEventHandler() {}
RoleKilledEventHandler::~RoleKilledEventHandler() {}

bool RoleKilledEventHandler::Initialize() {
  this->conn_actor_killed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleKilledEventHandler::OnEventRoleKilled,
        this, _1, _2, _3, _4));

  this->conn_npc_killed_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_KILLED, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&RoleKilledEventHandler::OnEventRoleKilled,
        this, _1, _2, _3, _4));

  return true;
}

void RoleKilledEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_killed_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_killed_);
}

void RoleKilledEventHandler::OnEventRoleKilled(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || size != sizeof(event::EventRoleKilled)) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleKilled *event = (event::EventRoleKilled *)message;

  if (event->attacker_type_ != entity::EntityType::TYPE_ACTOR) {
    return;
  }

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->attacker_id_);
  if (soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->attacker_id_);
    return;
  }

  SoulEnergy *soul_energy = soul_actor->GetSoulEnergy();
  if (soul_energy == NULL) {
    LOG_ERROR("获取 SoulEnergy(%lu) 失败", event->attacker_id_);
    return;
  }

  EnergyStatus *energy_status = soul_energy->GetCurrentStatus();
  if (energy_status == NULL || energy_status->GetType() != entity::SoulEnergyStatus::INCREMENT) {
    return;
  }

  GameActor *attacker_actor =
    GameActorManager::GetInstance()->GetActor(event->attacker_id_);
  if (attacker_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->attacker_id_);
    return;
  }

  core::int32 attacker_actor_level =
    attacker_actor->GetAttribute(entity::RoleAoiFields::LEVEL);

  if (event->type_ == entity::EntityType::TYPE_ACTOR) {
    GameActor *killed_actor = GameActorManager::GetInstance()->GetActor(event->id_);
    if (killed_actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", event->id_);
      return;
    }
    core::int32 killed_actor_level =
      killed_actor->GetAttribute(entity::RoleAoiFields::LEVEL);
    if (abs(attacker_actor_level - killed_actor_level) <
        MISC_CONF()->soul_energy_kill_actor_level_diff_) {
      attacker_actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY,
          attacker_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) +
          MISC_CONF()->soul_energy_kill_actor_energy_);
    }
  } else if (event->type_ == entity::EntityType::TYPE_NPC) {
    GameNpc *killed_npc = GameNpcManager::GetInstance()->Get(event->id_);
    if (killed_npc == NULL) {
      LOG_ERROR("获取 GameNpc(%lu) 失败", event->id_);
      return;
    }
    core::int32 killed_npc_level =
      killed_npc->GetAttribute(entity::RoleAoiFields::LEVEL);
    if (abs(attacker_actor_level - killed_npc_level) <
        MISC_CONF()->soul_energy_kill_actor_level_diff_) {
      attacker_actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY,
          attacker_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) +
          MISC_CONF()->soul_energy_kill_npc_energy_);
    }
  } else {
    return;
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game
