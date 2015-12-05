//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 12:18:33.
// File name: soul_setting.cc
//
// Description:
// Define class SoulSetting.
//

#include "game_server/server/extension/soul/soul_setting.h"

#include "game_server/server/extension/soul/configure.h"
#include "game_server/server/extension/soul/facade_request.h"
#include "game_server/server/extension/soul/soul.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_configure.h"
#include "game_server/server/extension/soul/soul_types.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace soul {

SoulSetting::SoulSetting() {}
SoulSetting::~SoulSetting() {}

bool SoulSetting::Initialize(SoulActor *actor,
    entity::VocationType::type vocation, core::int32 group, size_t size) {
  if (actor == NULL || vocation < entity::VocationType::MIN ||
      vocation >= entity::VocationType::MAX || size <= 0) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->actor_ = actor;
  this->settings_.resize(size, 0);
  this->vocation_ = vocation;
  this->group_ = group;
  this->stand_status_ = false;

  return true;
}

void SoulSetting::Finalize() {
  this->actor_ = NULL;
  this->settings_.clear();
}

bool SoulSetting::Stand() {
  if (this->stand_status_ == true) {
    LOG_ERROR("重复 stand");
    return false;
  }

  SettingVector::iterator iterator = this->settings_.begin();
  for (; iterator != this->settings_.end(); ++iterator) {
    core::int32 soul_id = *iterator;
    if (soul_id > 0) {
      Soul *soul = this->actor_->GetSoul(soul_id);
      if (soul == NULL) {
        LOG_ERROR("获取 玩家(%lu) Soul(%d) 失败",
            this->actor_->GetID(), soul_id);
        return false;
      }
      if (soul->Stand() == false) {
        LOG_ERROR("玩家(%lu) Soul(%d) stand 失败",
            this->actor_->GetID(), soul_id);
        return false;
      }
    }
  }

  core::uint32 skill_id = 0;
  switch (this->vocation_) {
    case entity::VocationType::AXE:
      skill_id = MISC_CONF()->axe_skill_;
      break;
    case entity::VocationType::SHOOT:
      skill_id = MISC_CONF()->shoot_skill_;
      break;
    case entity::VocationType::MAGIC:
      skill_id = MISC_CONF()->magic_skill_;
      break;
    default:
      break;
  }
  if (skill_id > 0) {
    // 开启职业技能
    if (FacadeRequest::GetInstance()->StandActorSkill(this->actor_->GetID(),
          skill_id) == false) {
      LOG_ERROR("请求启用职业技能失败");
      return false;
    }
  }

  this->stand_status_ = true;

  return true;
}

bool SoulSetting::Sit() {
  if (this->stand_status_ == false) {
    LOG_ERROR("重复 Sit");
    return false;
  }

  SettingVector::iterator iterator = this->settings_.begin();
  for (; iterator != this->settings_.end(); ++iterator) {
    core::int32 soul_id = *iterator;
    if (soul_id > 0) {
      Soul *soul = this->actor_->GetSoul(soul_id);
      if (soul == NULL) {
        LOG_ERROR("获取 玩家(%lu) Soul(%d) 失败",
            this->actor_->GetID(), soul_id);
        return false;
      }
      if (soul->Sit() == false) {
        LOG_ERROR("玩家(%lu) Soul(%d) sit 失败",
            this->actor_->GetID(), soul_id);
        return false;
      }
    }
  }

  this->stand_status_ = false;
  return true;
}

bool SoulSetting::SetSoul(Soul *soul, size_t pos) {
  if (soul == NULL || pos >= this->settings_.size()) {
    LOG_ERROR("参数错误");
    return false;
  }

  for (SettingVector::const_iterator iterator = this->settings_.begin();
      iterator != this->settings_.end(); ++iterator) {
    if (soul->GetID() == *iterator) {
      LOG_ERROR("玩家(%lu) 同一组(%d,%d) 多次设置英灵(%d)",
          this->actor_->GetID(), this->vocation_, this->group_, soul->GetID());
      return false;
    }
  }

  this->settings_[pos] = soul->GetID();

  if (this->stand_status_ == true) {
    if (soul->Stand() == false) {
      LOG_ERROR("玩家(%lu) Soul(%d) stand 失败", this->actor_->GetID(), soul->GetID());
      return false;
    }
  }

  return true;
}

bool SoulSetting::ResetSoul(size_t pos) {
  if (this->actor_ == NULL || pos >= this->settings_.size()) {
    LOG_ERROR("参数错误");
    return false;
  }

  core::int32 soul_id = this->settings_[pos];
  if (soul_id > 0) {
    Soul *soul = this->actor_->GetSoul(soul_id);
    if (soul == NULL) {
      LOG_ERROR("获取 玩家(%lu) Soul(%d) 失败", this->actor_->GetID(), soul_id);
      return false;
    }
    if (this->stand_status_ == true) {
      if (soul->Sit() == false) {
        LOG_ERROR("玩家(%lu) Soul(%d) sit 失败", this->actor_->GetID(), soul_id);
        return false;
      }
    }
  }

  this->settings_[pos] = 0;

  return true;
}

core::uint32 SoulSetting::GetSoul(size_t pos) const {
  if (pos >= this->settings_.size()) {
    return 0;
  }

  return this->settings_[pos];
}

const SoulSetting::SettingVector &SoulSetting::GetSettings() const {
  return this->settings_;
}

}  // name soul

}  // namespace server

}  // namespace game

