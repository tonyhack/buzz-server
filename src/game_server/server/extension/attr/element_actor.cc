#include "game_server/server/extension/attr/element_actor.h"

#include <cstddef>

#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/attr_actor.h"

namespace game {

namespace server {

namespace attr {

ElementActor::ElementActor() : attr_actor_(NULL), current_level_(0) {}
ElementActor::~ElementActor() {}

bool ElementActor::Initialize(AttrActor *attr_actor, const database::protocol::StorageAttrActorField &db_field) {
  if (NULL == attr_actor) {
    LOG_ERROR("attr_actor is null.");
    return false;
  }

  this->attr_actor_ = attr_actor;
  this->current_level_ = db_field.element_level_;

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return false;
  }

  // 未开启功能直接返回
  if (game_actor->CheckFunctionalityState(entity::FunctionalityType::ELEMENT_MODULE) == false) {
    return true;
  }

  this->current_level_ = 0;
  SetCurrentLevel(db_field.element_level_, false);

  // 发送初始化数据
  this->SendInitToClient();

  return true;
}

void ElementActor::Finalize() {
  this->attr_actor_ = NULL;
  this->current_level_ = 0;
}

void ElementActor::SendInitToClient() {
  gateway::protocol::MessageAttrElementInitialize message;
  message.__set_level_(this->current_level_);

  if (this->current_level_ != 0) {
    const ElementCell *cell = Configure::GetInstance()->GetElement(this->current_level_);
    if (NULL == cell) {
      LOG_ERROR("ElementCell(%d) not found in config.", this->current_level_);
      return;
    }
    message.__set_max_hp_add_value_(cell->max_hp_add_value_);
    message.__set_max_mp_add_value_(cell->max_mp_add_value_);
    message.__set_physics_attack_add_value_(cell->physics_attack_add_value_);
    message.__set_physics_defence_add_value_(cell->physics_defence_add_value_);
    message.__set_magic_attack_add_value_(cell->magic_attack_add_value_);
    message.__set_magic_defence_add_value_(cell->magic_defence_add_value_);
    message.__set_dodge_add_value_(cell->dodge_add_value_);
    message.__set_crit_add_value_(cell->crit_add_value_);
    message.__set_ignore_defence_add_value_(cell->ignore_defence_add_value_);
  }

  this->attr_actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_INITIALIZE);
}

void ElementActor::Save(database::protocol::StorageAttrActorField &db_field) {
  db_field.element_level_ = this->current_level_;
}

bool ElementActor::EnableElementFunc(bool synchronize) {
  if (synchronize) {
    this->SendInitToClient();
  }

  return true;
}

void ElementActor::SetCurrentLevel(core::int32 new_level, bool synchronize) {
  if (this->current_level_ == new_level) {
    return;
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

  const ElementCell *new_cell = Configure::GetInstance()->GetElement(new_level); 
  if (NULL == new_cell) {
      LOG_ERROR("ElementCell(%d) not found in config.", new_level);
      return;
  }
  const ElementCell *old_cell = Configure::GetInstance()->GetElement(this->current_level_);
  if (NULL == old_cell) {
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
    ignore_defence_add_value = new_cell->ignore_defence_add_value_ - old_cell->ignore_defence_add_value_;
  }

  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  this->current_level_ = new_level;
  if (max_hp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) +
        max_hp_add_value, synchronize);
  }
  if (max_mp_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) +
        max_mp_add_value, synchronize);
  }
  if (physics_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) +
        physics_attack_add_value, synchronize);
  }
  if (physics_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) +
        physics_defence_add_value, synchronize);
  }
  if (magic_attack_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) +
        magic_attack_add_value, synchronize);
  }
  if (magic_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) +
        magic_defence_add_value, synchronize);
  }
  if (dodge_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) +
        dodge_add_value, synchronize);
  }
  if (crit_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) +
        crit_add_value, synchronize);
  }
  if (ignore_defence_add_value != 0) {
    game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE, 
        game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) +
        ignore_defence_add_value, synchronize);
  }

  // 更新战斗力
  this->UpdateElementFightScore(synchronize);

  if (synchronize) {
    gateway::protocol::MessageAttrElementUpgradeResponse response;
    response.__set_result_(true);
    response.__set_level_(new_level);
    response.__set_max_hp_add_value_(new_cell->max_hp_add_value_);
    response.__set_max_mp_add_value_(new_cell->max_mp_add_value_);
    response.__set_physics_attack_add_value_(new_cell->physics_attack_add_value_);
    response.__set_physics_defence_add_value_(new_cell->physics_defence_add_value_);
    response.__set_magic_attack_add_value_(new_cell->magic_attack_add_value_);
    response.__set_magic_defence_add_value_(new_cell->magic_defence_add_value_);
    response.__set_dodge_add_value_(new_cell->dodge_add_value_);
    response.__set_crit_add_value_(new_cell->crit_add_value_);
    response.__set_ignore_defence_add_value_(new_cell->ignore_defence_add_value_);
    this->attr_actor_->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_UPGRADE_RESPONSE); 
  }
}

void ElementActor::UpdateElementFightScore(bool synchronize) {
  GameActor *game_actor = this->attr_actor_->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor is null.");
    return;
  }

  const ElementCell *cell = Configure::GetInstance()->GetElement(this->current_level_);
  if (NULL == cell) {
    LOG_ERROR("ElementCell(%d) not found in config.", this->current_level_);
    return;
  }

  FightScoreCalculator calc;
  calc.max_hp_ += cell->max_hp_add_value_;
  calc.max_mp_ += cell->max_mp_add_value_;
  calc.physics_attack_ += cell->physics_attack_add_value_;
  calc.physics_defence_ += cell->physics_defence_add_value_;
  calc.magic_attack_ += cell->magic_attack_add_value_;
  calc.magic_defence_ += cell->magic_defence_add_value_;
  calc.dodge_ += cell->dodge_add_value_;
  calc.crit_ += cell->crit_add_value_;
  calc.ignore_defence_ += cell->ignore_defence_add_value_;

  game_actor->SetSubFightScore(entity::SubFightScoreType::ELEMENT,
      calc.GetFightScore(), synchronize);
}

}  // namespace attr

}  // namespace server

}  // namespace game

