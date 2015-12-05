//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-08 15:29:53.
// File name: soul_energy.cc
//
// Description:
// Define class SoulEnergy.
//

#include "game_server/server/extension/soul/soul_energy.h"

#include "game_server/server/extension/soul/soul_actor.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace soul {

SoulEnergy::SoulEnergy() {}
SoulEnergy::~SoulEnergy() {}

bool SoulEnergy::Initialize(SoulActor *actor,
    const database::protocol::StorageSoulActorField &db_field) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  if (this->status_increment_.Initialize(this) == false) {
    LOG_ERROR("初始化 EnergyStatusIncrement 失败");
    return false;
  }
  if (this->status_explosion_.Initialize(this) == false) {
    LOG_ERROR("初始化 EnergyStatusExplosion 失败");
    return false;
  }
  if (this->status_decrement_.Initialize(this) == false) {
    LOG_ERROR("初始化 EnergyStatusDecrement 失败");
    return false;
  }

  this->actor_ = actor;
  this->load_current_energy_ = db_field.current_energy_;
  this->timer_start_ = false;
  this->current_status_ = NULL;

  return true;
}

void SoulEnergy::Finalize() {
  if (this->timer_start_ == true) {
    GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_ENERGY, this);
  }

  this->timer_start_ = false;
  this->actor_ = NULL;
  this->current_status_ = NULL;
}

void SoulEnergy::Save(database::protocol::StorageSoulActorField &db_field) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return;
  }

  db_field.__set_current_energy_(
      game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY));
}

bool SoulEnergy::OnLoadFinish() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 失败");
    return false;
  }

  game_actor->SetAttribute(entity::RoleClientFields::CURRENT_ENERGY, this->load_current_energy_);

  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_ENERGY,
      MISC_CONF()->soul_energy_interval_msecs_, -1, this, "SoulEnergy::OnLoadFinish");
  this->timer_start_ = true;

  // 能量状态机初始化
  if (game_actor->CheckCommonStatus(entity::RoleCommonStatus::SOUL) == true) {
    this->GotoStatus(entity::SoulEnergyStatus::DECREMENT);
  } else if (game_actor->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) ==
      game_actor->GetAttribute(entity::RoleClientFields::MAX_ENERGY)) {
    this->GotoStatus(entity::SoulEnergyStatus::EXPLOSION);
  } else {
    this->GotoStatus(entity::SoulEnergyStatus::INCREMENT);
  }
  this->current_status_->OnLoadFinish();

  return true;
}

void SoulEnergy::GotoStatus(entity::SoulEnergyStatus::type status) {
  if (this->current_status_ != NULL) {
    this->current_status_->OnLeave();
  }

  switch (status) {
    case entity::SoulEnergyStatus::INCREMENT:
      this->current_status_ = &this->status_increment_;
      break;
    case entity::SoulEnergyStatus::EXPLOSION:
      this->current_status_ = &this->status_explosion_;
      break;
    case entity::SoulEnergyStatus::DECREMENT:
      this->current_status_ = &this->status_decrement_;
      break;
    default:
      this->current_status_ = NULL;
      break;
  }

  if (this->current_status_ != NULL) {
    this->current_status_->OnEnter();
  }
}

void SoulEnergy::OnTimer(core::uint32 id) {
  if (this->current_status_ == NULL) {
    LOG_ERROR("current_status_ 为空");
    return;
  }

  this->current_status_->OnTimer();
}

}  // namespace soul

}  // namespace server

}  // namespace game

