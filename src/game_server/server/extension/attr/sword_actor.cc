//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 13:53:12.
// File name: sword_actor.cc
//
// Description:
// Define class SwordActor.
//

#include "game_server/server/extension/attr/sword_actor.h"

#include <cstddef>
#include <algorithm>

#include "entity/inform_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/facade_request.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace attr {

SwordActor::SwordActor() {}
SwordActor::~SwordActor() {}

bool SwordActor::Initialize(AttrActor *attr_actor,
    const database::protocol::StorageAttrActorField &db_field) {
  if(attr_actor == NULL) {
    LOG_ERROR("Parameter attr_actor is null.");
    return false;
  }

  this->attr_actor_ = attr_actor;

  this->SetAvailStep(db_field.sword_avail_step_);
  this->SetCurrentStep(db_field.sword_current_step_);
  this->SetLuckyValue(db_field.sword_lucky_value_);
  this->SetGainTime(db_field.sword_gain_time_);

  gateway::protocol::MessageAttrSwordInitialize msg;
  msg.__set_step_(this->GetAvailStep());
  msg.__set_lucky_value_(this->GetLuckyValue());
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", this->GetGainTime());
  msg.gain_time_ = buffer;
  attr_actor->SendMessage(msg,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_INITIALIZE);

  // 更新战斗力
  this->UpdateSwordFightScore(false);

  // 开启升阶定时器
  this->StartExpireTimer();

  return true;
}

void SwordActor::Finalize() {
  this->attr_actor_ = NULL;
  this->avail_step_ = 0;
  this->lucky_value_ = 0;
  this->gain_time_ = 0;
  GameServerSingleton::GetInstance().RemoveTimer(DISABLE_TIMER, this);
}

void SwordActor::Save(database::protocol::StorageAttrActorField &db_field) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("Member attr_actor_ is null.");
    return ;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor is null.");
    return ;
  }

  db_field.__set_sword_lucky_value_(this->GetLuckyValue());
  db_field.__set_sword_avail_step_(this->GetAvailStep());
  db_field.__set_sword_current_step_(game_actor->GetAttribute(
        entity::ActorAoiFields::CURRENT_SWORD));
  db_field.__set_sword_gain_time_(this->GetGainTime());
}

void SwordActor::SetAvailStep(core::int32 step) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("Member attr_actor_ is null.");
    return ;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor is null.");
    return ;
  }

  core::int32 current_step =
    game_actor->GetAttribute(entity::ActorAoiFields::CURRENT_SWORD);

  this->avail_step_ = step;

  // 如果 avail 变小了，需要把当前阶层设置成 avail
  if(current_step > step) {
    this->SetCurrentStep(step);
  }
}

core::int32 SwordActor::GetCurrentStep() const {
  if(this->attr_actor_ && this->attr_actor_->GetActor()) {
    return this->attr_actor_->GetActor()->GetAttribute(
        entity::ActorAoiFields::CURRENT_SWORD);
  }
  return 0;
}

void SwordActor::SetCurrentStep(core::int32 step) {
  if(this->attr_actor_ == NULL) {
    LOG_ERROR("Member attr_actor_ is null.");
    return ;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor is null.");
    return ;
  }

  core::int32 current_step =
    game_actor->GetAttribute(entity::ActorAoiFields::CURRENT_SWORD);

  if(step < 0 || current_step == step || step > this->GetAvailStep()) {
    return ;
  }

  core::int32 actor_level = game_actor->GetAttribute(entity::RoleAoiFields::LEVEL);

  core::int32 max_hp_add_value = 0;
  core::int32 max_mp_add_value = 0;
  core::int32 physics_attack_add_value = 0;
  core::int32 physics_defence_add_value = 0;
  core::int32 magic_attack_add_value = 0;
  core::int32 magic_defence_add_value = 0;
  core::int32 dodge_add_value = 0;
  core::int32 crit_add_value = 0;
  core::int32 ignore_defence_add_value = 0;

  if(step != 0) {
    const SwordCell *new_cell = Configure::GetInstance()->GetSword(step);
    if(new_cell == NULL) {
      LOG_ERROR("SwordCell(%d) not found in config.", step);
      return ;
    }
    const SwordCell *old_cell = Configure::GetInstance()->GetSword(current_step);
    if(old_cell == NULL) {
      max_hp_add_value = new_cell->max_hp_add_value_;
      max_mp_add_value = new_cell->max_mp_add_value_;
      physics_attack_add_value = new_cell->physics_attack_add_value_;
      physics_defence_add_value = new_cell->physics_defence_add_value_;
      magic_attack_add_value = new_cell->magic_attack_add_value_;
      magic_defence_add_value = new_cell->magic_defence_add_value_;
      dodge_add_value = new_cell->dodge_add_value_;
      crit_add_value = new_cell->crit_add_value_;
      ignore_defence_add_value = new_cell->ignore_defence_add_value_;
    } else {
      max_hp_add_value = new_cell->max_hp_add_value_ - old_cell->max_hp_add_value_;
      max_mp_add_value = new_cell->max_mp_add_value_ - old_cell->max_mp_add_value_;
      physics_attack_add_value = new_cell->physics_attack_add_value_ - old_cell->physics_attack_add_value_;
      physics_defence_add_value = new_cell->physics_defence_add_value_ - old_cell->physics_defence_add_value_;
      magic_attack_add_value = new_cell->magic_attack_add_value_ - old_cell->magic_attack_add_value_;
      magic_defence_add_value = new_cell->magic_defence_add_value_ - old_cell->magic_defence_add_value_;
      dodge_add_value = new_cell->dodge_add_value_ - old_cell->dodge_add_value_;
      crit_add_value = new_cell->crit_add_value_ - old_cell->crit_add_value_;
      ignore_defence_add_value = new_cell->ignore_defence_add_value_;
      // 删除之前阶层的魔剑BUFF到技能模块
      if(old_cell->buffs_.empty() == false) {
        if(FacadeRequest::GetInstance()->RemoveSkillGlobalDebuff(
              this->attr_actor_->GetID(), old_cell->buffs_) == false) {
          LOG_ERROR("RemoveSkilGlobalDebuff failed");
        }
      }
    }

    // 增加新阶层的魔剑BUFF到技能模块
    if(new_cell->buffs_.empty() == false) {
      if(FacadeRequest::GetInstance()->AddSkillGlobalDebuff(
            this->attr_actor_->GetID(), new_cell->buffs_) == false) {
        LOG_ERROR("AddSkilGlobalDebuff failed");
      }
    }

    // 等级设置
    this->SetLevel(new_cell->max_level_ < actor_level ? new_cell->max_level_ : actor_level);

  } else {
    const SwordCell *old_cell = Configure::GetInstance()->GetSword(current_step);
    if(old_cell) {
      max_hp_add_value = 0 - old_cell->max_hp_add_value_;
      max_mp_add_value = 0 - old_cell->max_mp_add_value_;
      physics_attack_add_value = 0 - old_cell->physics_attack_add_value_;
      physics_defence_add_value = 0 - old_cell->physics_defence_add_value_;
      magic_attack_add_value = 0 - old_cell->magic_attack_add_value_;
      magic_defence_add_value = 0 - old_cell->magic_defence_add_value_;
      dodge_add_value = 0 - old_cell->dodge_add_value_;
      crit_add_value = 0 - old_cell->crit_add_value_;
      ignore_defence_add_value = 0 - old_cell->ignore_defence_add_value_;
      // 删除之前阶层的魔剑BUFF到技能模块
      if(old_cell->buffs_.empty() == false) {
        if(FacadeRequest::GetInstance()->RemoveSkillGlobalDebuff(
              this->attr_actor_->GetID(), old_cell->buffs_) == false) {
          LOG_ERROR("RemoveSkilGlobalDebuff failed");
        }
      }
    }

    // 等级设置
    this->SetLevel(0);
  }

  this->attr_actor_->GetActor()->SetAttribute(entity::ActorAoiFields::CURRENT_SWORD, step);

  if(max_hp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) +
        max_hp_add_value);
  }
  if(max_mp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) +
        max_mp_add_value);
  }
  if(physics_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) +
        physics_attack_add_value);
  }
  if(physics_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) +
        physics_defence_add_value);
  }
  if(magic_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) +
        magic_attack_add_value);
  }
  if(magic_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) +
        magic_defence_add_value);
  }
  if(dodge_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) +
        dodge_add_value);
  }
  if(crit_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) +
        crit_add_value);
  }
  if(ignore_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) +
        ignore_defence_add_value);
  }
}

void SwordActor::SetLevel(core::int32 level) {
  if(this->GetAvailStep() <= 0) {
    return ;
  }

  if(level < 0) {
    return ;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor is null.");
    return ;
  }

  core::int32 max_hp_add_value = 0;
  core::int32 max_mp_add_value = 0;
  core::int32 physics_attack_add_value = 0;
  core::int32 physics_defence_add_value = 0;
  core::int32 magic_attack_add_value = 0;
  core::int32 magic_defence_add_value = 0;
  core::int32 dodge_add_value = 0;
  core::int32 crit_add_value = 0;
  core::int32 ignore_defence_add_value = 0;

  if(level != 0) {
    const SwordLevelCell *new_cell =
      Configure::GetInstance()->GetSwordLevel(level);
    if(new_cell == NULL) {
      LOG_ERROR("SwordLevelCell(%d) not found in config.", level);
      return ;
    }
    const SwordLevelCell *old_cell =
      Configure::GetInstance()->GetSwordLevel(this->GetLevel());
    if(old_cell == NULL) {
      max_hp_add_value = new_cell->max_hp_add_value_;
      max_mp_add_value = new_cell->max_mp_add_value_;
      physics_attack_add_value = new_cell->physics_attack_add_value_;
      physics_defence_add_value = new_cell->physics_defence_add_value_;
      magic_attack_add_value = new_cell->magic_attack_add_value_;
      magic_defence_add_value = new_cell->magic_defence_add_value_;
      dodge_add_value = new_cell->dodge_add_value_;
      crit_add_value = new_cell->crit_add_value_;
      ignore_defence_add_value = new_cell->ignore_defence_add_value_;
    } else {
      max_hp_add_value = new_cell->max_hp_add_value_ - old_cell->max_hp_add_value_;
      max_mp_add_value = new_cell->max_mp_add_value_ - old_cell->max_mp_add_value_;
      physics_attack_add_value = new_cell->physics_attack_add_value_ - old_cell->physics_attack_add_value_;
      physics_defence_add_value = new_cell->physics_defence_add_value_ - old_cell->physics_defence_add_value_;
      magic_attack_add_value = new_cell->magic_attack_add_value_ - old_cell->magic_attack_add_value_;
      magic_defence_add_value = new_cell->magic_defence_add_value_ - old_cell->magic_defence_add_value_;
      dodge_add_value = new_cell->dodge_add_value_ - old_cell->dodge_add_value_;
      crit_add_value = new_cell->crit_add_value_ - old_cell->crit_add_value_;
      ignore_defence_add_value = new_cell->ignore_defence_add_value_;
    }
  } else {
    const SwordLevelCell *old_cell =
      Configure::GetInstance()->GetSwordLevel(this->GetLevel());
    if(old_cell) {
      max_hp_add_value = 0 - old_cell->max_hp_add_value_;
      max_mp_add_value = 0 - old_cell->max_mp_add_value_;
      physics_attack_add_value = 0 - old_cell->physics_attack_add_value_;
      physics_defence_add_value = 0 - old_cell->physics_defence_add_value_;
      magic_attack_add_value = 0 - old_cell->magic_attack_add_value_;
      magic_defence_add_value = 0 - old_cell->magic_defence_add_value_;
      dodge_add_value = 0 - old_cell->dodge_add_value_;
      crit_add_value = 0 - old_cell->crit_add_value_;
      ignore_defence_add_value = old_cell->ignore_defence_add_value_;
    }
  }

  if(max_hp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) +
        max_hp_add_value);
  }
  if(max_mp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) +
        max_mp_add_value);
  }
  if(physics_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) +
        physics_attack_add_value);
  }
  if(physics_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) +
        physics_defence_add_value);
  }
  if(magic_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) +
        magic_attack_add_value);
  }
  if(magic_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) +
        magic_defence_add_value);
  }
  if(dodge_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) +
        dodge_add_value);
  }
  if(crit_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) +
        crit_add_value);
  }
  if(ignore_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) +
        ignore_defence_add_value);
  }

  this->level_ = level;
}

void SwordActor::DailyClean() {
  this->SetLuckyValue(0);
  if(this->attr_actor_) {
    LOG_ERROR("SwordActor[%lu] sword daily clean lucky value",
        this->attr_actor_->GetID());

    gateway::protocol::MessageAttrSwordSynchronize sync;
    sync.__set_lucky_value_(0);
    this->attr_actor_->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_SYNCHRONIZE);
  }
}

void SwordActor::StartExpireTimer() {
  if(this->attr_actor_ == NULL) {
    return ;
  }

  if(this->GetAvailStep() == 0) {
    return ;
  }

  // 先删除原来的定时器
  GameServerSingleton::GetInstance().RemoveTimer(DISABLE_TIMER, this);

  const SwordCell *cell = Configure::GetInstance()->GetSword(this->GetAvailStep());
  if(cell == NULL) {
    LOG_ERROR("Sword step[%u] not found in config.", this->GetAvailStep());
    return ;
  }

  if(cell->active_hours_ <= 0) {
    return ;
  }

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t expire = this->GetGainTime() + cell->active_hours_ * 60 * 60;

  // 已过期
  if(now >= expire) {
    LOG_ERROR("SwordActor[%lu] sword expired!", this->attr_actor_->GetID());
    this->SetCurrentStep(0);
    return ;
  }

  // 未过期
  GameServerSingleton::GetInstance().AddTimer(DISABLE_TIMER, (expire - now) * 1000,
      1, this, "SwordActor::StartExpireTimer");
}

void SwordActor::OnTimer(core::uint32 id) {
  if(id == DISABLE_TIMER) {
    this->SetCurrentStep(0);
  }
}

bool SwordActor::EnableSwordFunc() {
  if(this->attr_actor_ == NULL) {
    return false;
  }

  if(this->GetAvailStep() > 0) {
    return false;
  }

  this->SetAvailStep(1);
  this->SetGainTime(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());

  gateway::protocol::MessageAttrSwordInitialize message;
  message.__set_step_(this->GetAvailStep());
  message.__set_lucky_value_(this->GetLuckyValue());
  message.__set_gain_time_(global::ToString(this->GetGainTime()));
  this->attr_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_INITIALIZE);

  this->SetCurrentStep(1);
  // 更新战斗力
  this->UpdateSwordFightScore();

  this->StartExpireTimer();

  // 世界公告: 激活魔剑(神武)
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::ATTR_SWORD_ENABLE_FUNC);
  inform.params_.push_back(this->attr_actor_->GetStringID());
  inform.params_.push_back(this->attr_actor_->GetName());
  GameServerSingleton::GetInstance().BroadcastMessage(inform,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);

  return true;
}

void SwordActor::UpdateSwordFightScore(bool synchronize) {
  if(this->GetAvailStep() <= 0) {
    return ;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("GameActor is null.");
    return ;
  }

  core::int32 current_step =
    game_actor->GetAttribute(entity::ActorAoiFields::CURRENT_SWORD);

  const SwordCell *sword_cell = Configure::GetInstance()->GetSword(current_step);
  if(sword_cell == NULL) {
    LOG_ERROR("SwordCell(%d) not found in config.", current_step);
    return ;
  }

  core::int32 actor_level = game_actor->GetAttribute(entity::RoleAoiFields::LEVEL);
  core::int32 sword_level = std::min(actor_level, sword_cell->max_level_);

  const SwordLevelCell *sword_level_cell =
    Configure::GetInstance()->GetSwordLevel(this->GetLevel());
  if(sword_level_cell == NULL) {
    LOG_ERROR("SwordLevelCell(%d) not found in config.", this->GetLevel());
    return ;
  }

  FightScoreCalculator calc;
  calc.max_hp_ += sword_cell->max_hp_add_value_ + sword_level_cell->max_hp_add_value_;
  calc.max_mp_ += sword_cell->max_mp_add_value_ + sword_level_cell->max_mp_add_value_;
  calc.physics_attack_ += sword_cell->physics_attack_add_value_ +
      sword_level_cell->physics_attack_add_value_;
  calc.physics_defence_ += sword_cell->physics_defence_add_value_ +
      sword_level_cell->physics_defence_add_value_;
  calc.magic_attack_ += sword_cell->magic_attack_add_value_ +
      sword_level_cell->magic_attack_add_value_;
  calc.magic_defence_ += sword_cell->magic_defence_add_value_ +
      sword_level_cell->magic_defence_add_value_;
  calc.dodge_ += sword_cell->dodge_add_value_ +
      sword_level_cell->dodge_add_value_;
  calc.crit_ += sword_cell->crit_add_value_ +
      sword_level_cell->crit_add_value_;
  calc.ignore_defence_ += sword_cell->ignore_defence_add_value_ +
      sword_level_cell->ignore_defence_add_value_;

  game_actor->SetSubFightScore(entity::SubFightScoreType::SWORD,
      calc.GetFightScore(), synchronize);
}

}  // namespace attr

}  // namespace server

}  // namespace game

