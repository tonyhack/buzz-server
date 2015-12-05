//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-09 10:18:01.
// File name: soul_energy_status.cc
//
// Description:
// Define status of energy.
//

#include "game_server/server/extension/soul/soul_energy_status.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_energy.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace soul {

// TODO: 状态机初始化的时候，应该根据常见状态是否为SOUL以及energy是否满来判定进入哪个状态

bool EnergyStatus::Initialize(SoulEnergy *energy) {
  if (energy == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->energy_ = energy;

  return true;
}

void EnergyStatus::Finalize() {
  this->energy_ = NULL;
}

void EnergyStatusIncrement::OnEnter() {
  LOG_INFO("[EnergyStatus] EnergyStatusIncrement");
}

void EnergyStatusIncrement::OnLeave() {}

void EnergyStatusIncrement::OnLoadFinish() {}

void EnergyStatusIncrement::OnTimer() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  core::int32 current_energy =
    game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY);
  core::int32 increment =
    game_actor->GetAttribute(entity::RoleClientFields::ENERGY_INCREMENT);
  core::int32 max_energy =
    game_actor->GetAttribute(entity::RoleClientFields::MAX_ENERGY);

  if (current_energy < max_energy) {
    game_actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY,
        current_energy + increment);
  }

  this->Explode();
}

void EnergyStatusIncrement::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch (type) {
    case event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY:
      this->Explode();
      break;
    case event::EventType::EVENT_ROLE_CHANGE_MAX_ENERGY:
      this->Explode();
      break;
    default:
      break;
  }
}

void EnergyStatusIncrement::Explode() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  if (game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) ==
      game_actor->GetAttribute(entity::RoleClientFields::MAX_ENERGY)) {
    this->energy_->GotoStatus(entity::SoulEnergyStatus::EXPLOSION);
  }
}

void EnergyStatusExplosion::OnEnter() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  // 增加爆发BUFF
  if (FacadeRequest::GetInstance()->AddBuff(actor->GetID(),
        MISC_CONF()->soul_energy_explosion_buff_) == false) {
    LOG_ERROR("玩家(%lu) 增加 soul_energy_explosion_buff_ 失败");
    return;
  }

  LOG_INFO("[EnergyStatus] EnergyStatusExplosion");
}

void EnergyStatusExplosion::OnLeave() {}

void EnergyStatusExplosion::OnLoadFinish() {}

void EnergyStatusExplosion::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch (type) {
    case event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE:
      this->OnEventRoleCommonStatusChange(message, size);
      break;
    default:
      break;
  }
}

void EnergyStatusExplosion::OnEventRoleCommonStatusChange(
    const void *message, size_t size) {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  if (game_actor->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == true) {
    this->energy_->GotoStatus(entity::SoulEnergyStatus::DECREMENT);
  }
}

void EnergyStatusDecrement::OnEnter() {
  LOG_INFO("[EnergyStatus] EnergyStatusDecrement");
}

void EnergyStatusDecrement::OnLeave() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  // 增加爆发BUFF
  if (FacadeRequest::GetInstance()->RemoveBuff(actor->GetID(),
        MISC_CONF()->soul_energy_explosion_buff_) == false) {
    LOG_ERROR("玩家(%lu) 移除 soul_energy_explosion_buff_ 失败");
    return;
  }
}

void EnergyStatusDecrement::OnLoadFinish() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  // 增加爆发BUFF
  if (FacadeRequest::GetInstance()->AddBuff(actor->GetID(),
        MISC_CONF()->soul_energy_explosion_buff_) == false) {
    LOG_ERROR("玩家(%lu) 增加 soul_energy_explosion_buff_ 失败");
    return;
  }
}

void EnergyStatusDecrement::OnTimer() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  core::int32 current_energy =
    game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY);

  game_actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY,
      current_energy - MISC_CONF()->soul_energy_decrement_);

  this->Depleted();
}

void EnergyStatusDecrement::OnEvent(event::EventType::type type,
    const void *message, size_t size) {
  switch (type) {
    case event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY:
      this->Depleted();
      break;
    default:
      break;
  }
}

void EnergyStatusDecrement::Depleted() {
  if (this->energy_ == NULL) {
    LOG_ERROR("energy_ 为空");
    return;
  }

  SoulActor *actor = this->energy_->GetActor();
  if (actor == NULL) {
    LOG_ERROR("获取 SoulActor 失败");
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  if (game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) == 0) {
    this->energy_->GotoStatus(entity::SoulEnergyStatus::INCREMENT);
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

