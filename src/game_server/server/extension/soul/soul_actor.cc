//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 14:51:35.
// File name: soul_actor.cc
//
// Description:
// Define class SoulActor.
//

#include "game_server/server/extension/soul/soul_actor.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_soul_actor_save_types.h"
#include "database_server/protocol/storage_soul_login_types.h"
#include "database_server/protocol/storage_soul_save_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/soul_pool.h"
#include "gateway_server/protocol/gateway_soul_protocol_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace soul {

SoulActor::SoulActor() {}

SoulActor::~SoulActor() {}

bool SoulActor::Initialize(GameActor *actor,
    const database::protocol::StorageSoulActorField &field) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->actor_ = actor;

  // 快捷设置
  for (core::int32 vocation = entity::VocationType::MIN;
      vocation < entity::VocationType::MAX; ++vocation) {
    for (core::int32 group = 0; group <= kSettingNum; ++group) {
      SoulSettingKey key((entity::VocationType::type)vocation, group);
      if (this->settings_.find(key) != this->settings_.end()) {
        LOG_ERROR("玩家(%lu) 重复英灵设置(%d,%d)",
            this->GetID(), key.first, key.second);
        return false;
      }
      SoulSetting setting;
      core::int32 setting_size = kSettingGroupNum;
      if (group == 0) {
        setting_size = kSettingSoulGroupNum;
      }
      if (setting.Initialize(this, key.first, key.second, setting_size) == false) {
        LOG_ERROR("初始化玩家(%lu) 英灵设置(%d,%d) 失败",
            this->GetID(), key.first, key.second);
        return false;
      }
      this->settings_.insert(std::make_pair(key, setting));
    }
  }

  this->axe_stand_group_ = field.axe_setting_ <= 0 ? 1 : field.axe_setting_;
  this->shoot_stand_group_ = field.shoot_setting_ <= 0 ? 1 : field.shoot_setting_;
  this->magic_stand_group_ = field.magic_setting_ <= 0 ? 1 : field.magic_setting_;

  entity::VocationType::type vocation =
    (entity::VocationType::type)this->actor_->GetAttribute(entity::RoleAoiFields::VOCATION);
  if (vocation == entity::VocationType::AXE) {
    this->current_setting_.second = this->axe_stand_group_;
  } else if (vocation == entity::VocationType::SHOOT) {
    this->current_setting_.second = this->shoot_stand_group_;
  } else if (vocation == entity::VocationType::MAGIC) {
    this->current_setting_.second = this->magic_stand_group_;
  } else {
    LOG_ERROR("玩家(%lu) 当前职业(%d) 错误", actor->GetID(), vocation);
    return false;
  }
  this->current_setting_.first = vocation;
  this->setting_fields_ = field.settings_;

  if(this->soul_altar_actor_.Initialize(this, field) == false) {
    LOG_ERROR("初始化 SoulAltarActor 失败");
    return false;
  }

  if (this->soul_energy_.Initialize(this, field) == false) {
    LOG_ERROR("初始化 SoulEnergy 失败");
    return false;
  }

  this->load_finish_ = false;
  this->fighting_status_ = false;

  return true;
}

void SoulActor::Finalize() {
  this->soul_energy_.Finalize();
  this->soul_altar_actor_.Finalize();

  for (SoulSettingMap::iterator iterator = this->settings_.begin();
      iterator != this->settings_.end(); ++iterator) {
    iterator->second.Finalize();
  }

  for (SoulMap::iterator iterator = this->souls_.begin();
      iterator != this->souls_.end(); ++iterator) {
    iterator->second->Finalize();
    SoulPool::GetInstance()->Deallocate(iterator->second);
  }

  this->settings_.clear();
  this->souls_.clear();
  this->actor_ = NULL;
}

bool SoulActor::ImportSouls(const SoulVector &souls) {
  SoulVector::const_iterator iterator = souls.begin();
  for (; iterator != souls.end(); ++iterator) {
    const SoulCell *cell = Configure::GetInstance()->GetSoulCell(
        iterator->id_);
    if (cell == NULL) {
      LOG_ERROR("玩家(%lu) 初始化时找不到英灵(%u)配置",
          this->GetID(), iterator->id_);
      return false;
    }
    Soul *soul = SoulPool::GetInstance()->Allocate();
    if (soul == NULL) {
      LOG_ERROR("玩家(%lu) 初始化时分配英灵失败", this->actor_->GetID());
      return false;
    }
    if (soul->Initialize(this, iterator->id_) == false) {
      SoulPool::GetInstance()->Deallocate(soul);
      LOG_ERROR("玩家(%lu) 初始化英灵失败", this->actor_->GetID());
      return false;
    }
    soul->SetStep(iterator->step_);
    soul->SetLevel(iterator->level_);
    if (this->AddSoul(soul) == false) {
      soul->Finalize();
      SoulPool::GetInstance()->Deallocate(soul);
      LOG_ERROR("玩家(%lu) 加入英灵失败", this->actor_->GetID());
      return false;
    }
    if (cell->common_skill_ > 0) {
      soul->SetCommonSkillLevel(iterator->common_skill_level_);
      FacadeRequest::GetInstance()->AddActorSkill(this->GetID(), cell->common_skill_,
          soul->GetCommonSkillLevel(), false);
    }
    if (cell->appear_skill_ > 0) {
      soul->SetAppearSkillLevel(iterator->appear_skill_level_);
      FacadeRequest::GetInstance()->AddActorSkill(this->GetID(), cell->appear_skill_,
          soul->GetAppearSkillLevel(), false);
    }
    if (cell->soul_skill_ > 0) {
      soul->SetSoulSkillLevel(iterator->soul_skill_level_);
      FacadeRequest::GetInstance()->AddActorSkill(this->GetID(), cell->soul_skill_,
          soul->GetSoulSkillLevel(), false);
    }

    LOG_DEBUG("玩家(%lu) 增加英灵(%u) 成功", this->actor_->GetID(), soul->GetID());
  }

  // 请求增加职业技能
  if(FacadeRequest::GetInstance()->AddActorSkill(this->GetID(),
      MISC_CONF()->axe_skill_, MISC_CONF()->axe_skill_level_) == false) {
    LOG_ERROR("请求启用职业技能失败");
    return false;
  }
  if(FacadeRequest::GetInstance()->AddActorSkill(this->GetID(),
      MISC_CONF()->shoot_skill_, MISC_CONF()->shoot_skill_level_) == false) {
    LOG_ERROR("请求启用职业技能失败");
    return false;
  }
  if(FacadeRequest::GetInstance()->AddActorSkill(this->GetID(),
      MISC_CONF()->magic_skill_, MISC_CONF()->magic_skill_level_) == false) {
    LOG_ERROR("请求启用职业技能失败");
    return false;
  }

  // 更新战斗力
  this->UpdateSoulFightScore(false);

  return true;
}

void SoulActor::Synchronize() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  gateway::protocol::MessageSoulInitialize message;
  message.__set_axe_group_(this->axe_stand_group_);
  message.__set_shoot_group_(this->shoot_stand_group_);
  message.__set_magic_group_(this->magic_stand_group_);

  entity::SoulSettingField setting_field;
  for (SoulSettingMap::iterator iterator = this->settings_.begin();
      iterator != this->settings_.end(); ++iterator) {
    setting_field.__set_vocation_(iterator->first.first);
    setting_field.__set_group_(iterator->first.second);
    setting_field.__set_settings_(iterator->second.GetSettings());
    message.settings_.push_back(setting_field);
  }

  entity::SoulField soul_field;
  for (SoulMap::iterator iterator = this->souls_.begin();
      iterator != this->souls_.end(); ++iterator) {
    Soul *soul = iterator->second;
    if (soul == NULL) {
      LOG_ERROR("玩家(%lu) 英灵表中为空指针", this->actor_->GetID());
      return;
    }
    soul_field.__set_id_(soul->GetID());
    soul_field.__set_level_(soul->GetLevel());
    soul_field.__set_step_(soul->GetStep());
    soul_field.__set_common_skill_level_(soul->GetCommonSkillLevel());
    soul_field.__set_appear_skill_level_(soul->GetAppearSkillLevel());
    soul_field.__set_soul_skill_level_(soul->GetSoulSkillLevel());
    message.souls_.push_back(soul_field);
  }


  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_SOUL_INITIALIZE);
}

void SoulActor::Save() {
  if (this->CheckLoadFinish() == false) {
    return;
  }
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }
  if (this->souls_.empty() == true) {
    return;
  }

  database::protocol::StorageSoulSaveRequest request_soul_save;
  database::protocol::StorageSoulField field;
  request_soul_save.__set_id_(this->GetID());

  for (SoulMap::const_iterator iterator = this->souls_.begin();
      iterator != this->souls_.end(); ++iterator) {
    Soul *soul = iterator->second;
    if (soul == NULL) {
      LOG_ERROR("玩家(%lu) 英灵表中为空指针", this->actor_->GetID());
      return;
    }
    field.__set_id_(soul->GetID());
    field.__set_level_(soul->GetLevel());
    field.__set_step_(soul->GetStep());
    field.__set_common_skill_level_(soul->GetCommonSkillLevel());
    field.__set_appear_skill_level_(soul->GetAppearSkillLevel());
    field.__set_soul_skill_level_(soul->GetSoulSkillLevel());
    request_soul_save.fields_.push_back(field);
  }

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request_soul_save, database::protocol::StorageType::STORAGE_SOUL_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("玩家(%lu) 保存英灵信息失败", this->actor_->GetID());
    return ;
  }

  database::protocol::StorageSoulActorSaveRequest request_soul_actor_save;
  request_soul_actor_save.__set_actor_id_(this->GetID());
  request_soul_actor_save.field_.__set_axe_setting_(this->axe_stand_group_);
  request_soul_actor_save.field_.__set_shoot_setting_(this->shoot_stand_group_);
  request_soul_actor_save.field_.__set_magic_setting_(this->magic_stand_group_);
  for (SoulSettingMap::const_iterator iterator = this->settings_.begin();
      iterator != this->settings_.end(); ++iterator) {
    request_soul_actor_save.field_.settings_ += global::ToString(iterator->first.first);
    request_soul_actor_save.field_.settings_ += "+";
    request_soul_actor_save.field_.settings_ += global::ToString(iterator->first.second);
    request_soul_actor_save.field_.settings_ += "+";
    const SoulSetting::SettingVector &settings = iterator->second.GetSettings();
    request_soul_actor_save.field_.settings_ += global::ToString(settings.size());
    request_soul_actor_save.field_.settings_ += "+";
    for (size_t pos = 0; pos < settings.size(); ++pos) {
      request_soul_actor_save.field_.settings_ += global::ToString(settings[pos]);
      request_soul_actor_save.field_.settings_ += "+";
    }
    request_soul_actor_save.field_.settings_ += ",";
  }
  
  soul_altar_actor_.Save(request_soul_actor_save.field_);
  this->soul_energy_.Save(request_soul_actor_save.field_);

  // 发送消息
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request_soul_actor_save, database::protocol::StorageType::STORAGE_SOUL_ACTOR_SAVE,
        this->actor_->GetID()) == false) {
    LOG_ERROR("玩家(%lu) 保存英灵信息失败", this->actor_->GetID());
    return ;
  }

  LOG_DEBUG("玩家(%lu) 保存英灵信息成功", this->actor_->GetID());
}

bool SoulActor::LoadFinish() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  // 解析快捷设置
  std::vector<std::string> settings_str;
  std::vector<core::int32> settings_field;
  global::StringSplit(this->setting_fields_.c_str(), ",", settings_str);
  for (size_t pos = 0; pos < settings_str.size(); ++pos) {
    global::StringSplit(settings_str[pos].c_str(), "+", settings_field);
    if (settings_field.size() < 3) {
      LOG_ERROR("玩家(%lu) 解析设置出错", this->GetID());
      return false;
    }
    SoulSetting *setting = this->GetSetting(
        (entity::VocationType::type)settings_field[0], settings_field[1]);
    if (setting == NULL) {
      LOG_ERROR("玩家(%lu) 解析设置(%d,%d) 不存在", this->GetID(),
          settings_field[0], settings_field[1]);
      return false;
    }
    for (size_t pos = 0; pos < (size_t)settings_field[2]; ++pos) {
      core::int32 soul_id = settings_field[3 + pos];
      if (soul_id == 0) {
        continue;
      }
      Soul *soul = this->GetSoul(soul_id);
      if (soul == NULL) {
        LOG_ERROR("玩家(%lu) 解析设置(%d,%d)时，获取英灵(%d) 失败",
            this->GetID(), settings_field[0], settings_field[1], soul_id);
        return false;
      }
      if (setting->SetSoul(soul, pos) == false) {
        LOG_ERROR("玩家(%lu) 解析设置(%d,%d)时，获取英灵(%d) 失败",
            this->GetID(), settings_field[0], settings_field[1], soul_id);
        return false;
      }
    }
  }

  SoulSettingMap::iterator iterator =
    this->settings_.find(this->current_setting_);
  if (iterator == this->settings_.end()) {
    LOG_ERROR("玩家(%lu) 找不到当前设置(%d,%d)", this->GetID(),
        this->current_setting_.first, this->current_setting_.second);
    return false;
  }
  if (iterator->second.Stand() == false) {
    LOG_ERROR("玩家(%lu) Stand 当前设置(%d,%d) 失败", this->GetID(),
        this->current_setting_.first, this->current_setting_.second);
    return false;
  }

  SoulSetting *appear_setting = this->GetSetting(
      (entity::VocationType::type)this->actor_->GetAttribute(entity::RoleAoiFields::VOCATION), 0);
  if (appear_setting == NULL) {
    LOG_ERROR("玩家(%lu) 获取幻化设置失败", this->GetID());
    return false;
  }
  if (appear_setting->Stand() == false) {
    LOG_ERROR("玩家(%lu) Stand 幻化设置失败", this->GetID());
    return false;
  }

  if (this->soul_energy_.OnLoadFinish() == false) {
    LOG_ERROR("玩家(%lu) SoulEnergy 加载完成时失败", this->GetID());
    return false;
  }

  this->Synchronize();

  this->load_finish_ = true;

  return true;
}

static void AddAttributeToCalculator(FightScoreCalculator *calc,
    entity::SoulAttributeType::type type, core::int32 value) {
  switch(type) {
    case entity::SoulAttributeType::MAX_HP:
      calc->max_hp_ += value;
      break;
    case entity::SoulAttributeType::MAX_MP:
      calc->max_mp_ += value;
      break;
    case entity::SoulAttributeType::PHYSICS_ATTACK:
      calc->physics_attack_ += value;
      break;
    case entity::SoulAttributeType::PHYSICS_DEFENCE:
      calc->physics_defence_ += value;
      break;
    case entity::SoulAttributeType::MAGIC_ATTACK:
      calc->magic_attack_ += value;
      break;
    case entity::SoulAttributeType::MAGIC_DEFENCE:
      calc->magic_defence_ += value;
      break;
    case entity::SoulAttributeType::DODGE:
      calc->dodge_ += value;
      break;
    case entity::SoulAttributeType::CRIT:
      calc->crit_ += value;
      break;
    case entity::SoulAttributeType::IGNORE_DEFENCE:
      calc->ignore_defence_ += value;
      break;
    default:
      break;
  }
}

void SoulActor::UpdateSoulFightScore(bool sync) {
  if(this->actor_ == NULL) {
    LOG_ERROR("Actor is null.");
    return ;
  }

  FightScoreCalculator calc;

  SoulMap::const_iterator iterator = this->souls_.begin();
  for(; iterator != this->souls_.end(); ++iterator) {
    const Soul *soul = iterator->second;
    const SoulCell *soul_cell = Configure::GetInstance()->GetSoulCell(
        soul->GetID());
    if(soul_cell == NULL) {
      LOG_ERROR("英灵(%u) 找不到对应配置", soul->GetID());
      continue;
    }
    const SoulStepCell *soul_step_cell =
      Configure::GetInstance()->GetSoulStepCell(soul_cell->step_group_,
          soul->GetLevel(), soul->GetStep());
    if (soul_step_cell == NULL) {
      LOG_ERROR("获取 SoulStepCell(%d,%d,%d) 失败", soul_cell->step_group_,
          soul->GetLevel(), soul->GetStep());
      continue;
    }
    for (SoulStepCell::AttrMap::const_iterator iterator = soul_step_cell->attrs_.begin();
        iterator != soul_step_cell->attrs_.end(); ++iterator) {
      AddAttributeToCalculator(&calc, iterator->first, iterator->second);
    }
    calc.skill_level_ += soul->GetCommonSkillLevel();
    calc.skill_level_ += soul->GetAppearSkillLevel();
    calc.skill_level_ += soul->GetSoulSkillLevel();
  }

  this->actor_->SetSubFightScore(entity::SubFightScoreType::SOUL,
      calc.GetFightScore(), sync);
}

bool SoulActor::AddSoul(Soul *soul) {
  if (soul == NULL || this->actor_ == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  const SoulCell *soul_cell = Configure::GetInstance()->GetSoulCell(
      soul->GetID());
  if(soul_cell == NULL) {
    LOG_ERROR("英灵(%u) 找不到对应配置", soul->GetID());
    return false;
  }
  const SoulStepCell *soul_step_cell =
    Configure::GetInstance()->GetSoulStepCell(soul_cell->step_group_,
        soul->GetLevel(), soul->GetStep());
  if (soul_step_cell == NULL) {
    LOG_ERROR("获取 SoulStepCell(%d,%d,%d) 失败", soul_cell->step_group_,
        soul->GetLevel(), soul->GetStep());
    return false;
  }

  SoulMap::const_iterator iterator = this->souls_.find(soul->GetID());
  if (iterator != this->souls_.end()) {
    return false;
  }

  this->souls_.insert(std::make_pair(soul->GetID(), soul));

  for (SoulStepCell::AttrMap::const_iterator iterator = soul_step_cell->attrs_.begin();
      iterator != soul_step_cell->attrs_.end(); ++iterator) {
    soul->AddAttr(iterator->first, iterator->second);
  }

  return true;
}

Soul *SoulActor::GetSoul(core::int32 soul_id) {
  SoulMap::iterator iterator = this->souls_.find(soul_id);
  if (iterator != this->souls_.end()) {
    return iterator->second;
  }
  return NULL;
}

SoulSetting *SoulActor::GetSetting(entity::VocationType::type vocation,
    core::int32 group) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return NULL;
  }

  SoulSettingMap::iterator iterator =
    this->settings_.find(SoulSettingKey(vocation, group));
  if (iterator == this->settings_.end()) {
    LOG_ERROR("获取玩家(%lu) 设置(%d,%d) 失败", this->GetID(), vocation, group);
    return NULL;
  }

  return &iterator->second;
}

const SoulSetting *SoulActor::GetSetting(entity::VocationType::type vocation,
    core::int32 group) const {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return NULL;
  }

  SoulSettingMap::const_iterator iterator =
    this->settings_.find(SoulSettingKey(vocation, group));
  if (iterator == this->settings_.end()) {
    LOG_ERROR("获取玩家(%lu) 设置(%d,%d) 失败", this->GetID(), vocation, group);
    return NULL;
  }

  return &iterator->second;
}

void SoulActor::DailyClean() {
  this->soul_altar_actor_.DailyClean();
}

}  // namespace soul

}  // namespace server

}  // namespace game

