//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 10:56:05.
// File name: soul.cc
//
// Description:
// Define class Soul.
//

#include "game_server/server/extension/soul/soul.h"

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_effect_request_types.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace soul {

Soul::Soul() {}
Soul::~Soul() {}

bool Soul::Initialize(SoulActor *actor, core::int32 soul_id) {
  if (actor == NULL || soul_id <= 0) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->actor_ = actor;
  this->id_ = soul_id;
  this->step_ = 0;
  this->level_ = 0;
  this->common_skill_level_ = 0;
  this->appear_skill_level_ = 0;
  this->soul_skill_level_ = 0;
  this->stand_status_ = false;
  this->setting_status_ = false;

  return true;
}

void Soul::Finalize() {
  this->actor_ = NULL;
}

void Soul::Synchronize() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  gateway::protocol::MessageSoulSynchronize message;
  message.soul_.__set_id_(this->GetID());
  message.soul_.__set_level_(this->GetLevel());
  message.soul_.__set_step_(this->GetStep());
  message.soul_.__set_common_skill_level_(this->GetCommonSkillLevel());
  message.soul_.__set_appear_skill_level_(this->GetAppearSkillLevel());
  message.soul_.__set_soul_skill_level_(this->GetSoulSkillLevel());

  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SOUL_SYNCHRONIZE);
}

bool Soul::Stand() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  if (this->GetStandStatus() == true) {
    LOG_ERROR("已处于 stand 状态");
    return false;
  }

  const SoulCell *cell =
    Configure::GetInstance()->GetSoulCell(this->id_);
  if (cell == NULL) {
    LOG_ERROR("获取 SoulCell(%d) 失败", this->id_);
    return false;
  }

  // 开启技能
  if (cell->common_skill_ > 0) {
    if (FacadeRequest::GetInstance()->StandActorSkill(this->actor_->GetID(),
          cell->common_skill_) == false) {
      LOG_ERROR("请求启用 common_skill 失败");
      return false;
    }
  }
  if (cell->appear_skill_ > 0) {
    if (FacadeRequest::GetInstance()->StandActorSkill(this->actor_->GetID(),
          cell->appear_skill_) == false) {
      LOG_ERROR("请求启用 appear_skill 失败");
      return false;
    }
  }
  if (cell->soul_skill_ > 0) {
    if (FacadeRequest::GetInstance()->StandActorSkill(this->actor_->GetID(),
          cell->soul_skill_) == false) {
      LOG_ERROR("请求启用 soul_skill 失败");
      return false;
    }
  }

  // 开启精灵效果
  core::int32 spirit_level = FacadeRequest::GetInstance()->GetSpiritLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->spirit_effects_.begin();
      iterator != cell->spirit_effects_.end(); ++iterator) {
    if (iterator->first > spirit_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StartStandEffect(iterator->second[pos]);
    }
  }
  // 开启魔剑效果
  core::int32 sword_step = FacadeRequest::GetInstance()->GetSwordStep(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->sword_effects_.begin();
      iterator != cell->sword_effects_.end(); ++iterator) {
    if (iterator->first > sword_step) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StartStandEffect(iterator->second[pos]);
    }
  }
  // 开启贵族效果
  core::int32 noble_level = FacadeRequest::GetInstance()->GetNobleLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->noble_effects_.begin();
      iterator != cell->noble_effects_.end(); ++iterator) {
    if (iterator->first > noble_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StartStandEffect(iterator->second[pos]);
    }
  }

  this->SetStandStatus(true);
  this->SetSettingStatus(true);

  return true;
}

bool Soul::Sit() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  const SoulCell *cell =
    Configure::GetInstance()->GetSoulCell(this->id_);
  if (cell == NULL) {
    LOG_ERROR("获取 SoulCell(%d) 失败", this->id_);
    return false;
  }

  if (this->GetStandStatus() == false) {
    LOG_ERROR("已处于 sit 状态");
    return false;
  }

  // 关闭精灵效果
  core::int32 spirit_level = FacadeRequest::GetInstance()->GetSpiritLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->spirit_effects_.begin();
      iterator != cell->spirit_effects_.end(); ++iterator) {
    if (iterator->first > spirit_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StopStandEffect(iterator->second[pos]);
    }
  }
  // 关闭魔剑效果
  core::int32 sword_step = FacadeRequest::GetInstance()->GetSwordStep(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->sword_effects_.begin();
      iterator != cell->sword_effects_.end(); ++iterator) {
    if (iterator->first > sword_step) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StopStandEffect(iterator->second[pos]);
    }
  }
  // 关闭贵族效果
  core::int32 noble_level = FacadeRequest::GetInstance()->GetNobleLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->noble_effects_.begin();
      iterator != cell->noble_effects_.end(); ++iterator) {
    if (iterator->first > noble_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      this->StopStandEffect(iterator->second[pos]);
    }
  }

  // 关闭技能
  if (cell->common_skill_ > 0) {
    if (FacadeRequest::GetInstance()->SitActorSkill(this->actor_->GetID(),
          cell->common_skill_) == false) {
      LOG_ERROR("请求启用 common_skill 失败");
      return false;
    }
  }
  if (cell->appear_skill_ > 0) {
    if (FacadeRequest::GetInstance()->SitActorSkill(this->actor_->GetID(),
          cell->appear_skill_) == false) {
      LOG_ERROR("请求启用 appear_skill 失败");
      return false;
    }
  }
  if (cell->soul_skill_ > 0) {
    if (FacadeRequest::GetInstance()->SitActorSkill(this->actor_->GetID(),
          cell->soul_skill_) == false) {
      LOG_ERROR("请求启用 soul_skill 失败");
      return false;
    }
  }

  this->SetStandStatus(false);
  this->SetSettingStatus(false);

  return true;
}

bool Soul::StartStandEffect(const SoulEffectValue &effect) {
  if (effect.type_ == entity::SoulEffectType::ATTR) {
    return this->AddAttr((entity::SoulAttributeType::type)effect.param1_, effect.param2_);
  } else if (effect.type_ == entity::SoulEffectType::EFFECT) {
    return this->AddEffect(effect.param1_);
  } else if (effect.type_ == entity::SoulEffectType::SKILL_MAX_LEVEL) {
    return true;
  } else {
    LOG_ERROR("类型(%d)错误", effect.type_);
    return false;
  }

  return true;
}

bool Soul::StopStandEffect(const SoulEffectValue &effect) {
  if (effect.type_ == entity::SoulEffectType::ATTR) {
    return this->AddAttr((entity::SoulAttributeType::type)effect.param1_, 0 - effect.param2_);
  } else if (effect.type_ == entity::SoulEffectType::EFFECT) {
    return this->RemoveEffect(effect.param1_);
  } else if (effect.type_ == entity::SoulEffectType::SKILL_MAX_LEVEL) {
    return true;
  } else {
    LOG_ERROR("类型(%d)错误", effect.type_);
    return false;
  }

  return true;
}

bool Soul::AddAttr(entity::SoulAttributeType::type type, core::int32 increament) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  GameActor *game_actor = this->actor_->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor 为空");
    return false;
  }

  switch (type) {
    case entity::SoulAttributeType::MAX_HP:
      game_actor->SetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAX_HP_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::MAX_MP:
      game_actor->SetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAX_MP_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::PHYSICS_ATTACK:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_ATTACK_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::PHYSICS_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::PHYSICS_DEFENCE_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::MAGIC_ATTACK:
      game_actor->SetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAGIC_ATTACK_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::MAGIC_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::MAGIC_DEFENCE_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::DODGE:
      game_actor->SetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::DODGE_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::CRIT:
      game_actor->SetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::CRIT_ADD_VALUE) + increament);
      break;
    case entity::SoulAttributeType::IGNORE_DEFENCE:
      game_actor->SetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE,
          game_actor->GetAttribute(entity::RoleServerFields::IGNORE_DEFENCE_ADD_VALUE) + increament);
      break;
    default:
      break;
  }

  return true;
}

bool Soul::AddEffect(core::int32 effect) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  request::RequestEffectStart request;
  request.__set_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_entity_id_(this->actor_->GetID());
  request.__set_effect_id_(effect);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_EFFECT_START,
        &request, sizeof(request)) != 0 || request.succeed_ == false) {
    LOG_ERROR("玩家(%lu) Soul(%d) 开启效果(%d) 失败",
        this->actor_->GetID(), this->GetID(), effect);
    return false;
  }

  return true;
}

bool Soul::RemoveEffect(core::int32 effect) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  request::RequestEffectEnd request;
  request.__set_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_entity_id_(this->actor_->GetID());
  request.__set_effect_id_(effect);
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_EFFECT_END,
        &request, sizeof(request)) != 0 || request.succeed_ == false) {
    LOG_ERROR("玩家(%lu) Soul(%d) 开启效果(%d) 失败",
        this->actor_->GetID(), this->GetID(), effect);
    return false;
  }

  return true;
}

core::int32 Soul::GetMaxSkillLevel() const{
  core::int32 max_level = 0;

  const SoulCell *cell =
    Configure::GetInstance()->GetSoulCell(this->id_);
  if (cell == NULL) {
    LOG_ERROR("获取 SoulCell(%d) 失败", this->id_);
    return false;
  }
  core::int32 spirit_level = FacadeRequest::GetInstance()->GetSpiritLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->spirit_effects_.begin();
      iterator != cell->spirit_effects_.end(); ++iterator) {
    if (iterator->first > spirit_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      if (iterator->second[pos].type_ == entity::SoulEffectType::SKILL_MAX_LEVEL) {
        max_level += iterator->second[pos].param1_;
      }
    }
  }
  core::int32 sword_step = FacadeRequest::GetInstance()->GetSwordStep(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->sword_effects_.begin();
      iterator != cell->sword_effects_.end(); ++iterator) {
    if (iterator->first > sword_step) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      if (iterator->second[pos].type_ == entity::SoulEffectType::SKILL_MAX_LEVEL) {
        max_level += iterator->second[pos].param1_;
      }
    }
  }
  core::int32 noble_level = FacadeRequest::GetInstance()->GetNobleLevel(this->actor_->GetID());
  for (SoulCell::EffectMap::const_iterator iterator = cell->noble_effects_.begin();
      iterator != cell->noble_effects_.end(); ++iterator) {
    if (iterator->first > noble_level) {
      break;
    }
    for (size_t pos = 0; pos < iterator->second.size(); ++pos) {
      if (iterator->second[pos].type_ == entity::SoulEffectType::SKILL_MAX_LEVEL) {
        max_level += iterator->second[pos].param1_;
      }
    }
  }

  return max_level + this->GetLevel() * kSoulLevelUpgradeSkillLevel;
}

}  // namespace soul

}  // namespace server

}  // namespace game

