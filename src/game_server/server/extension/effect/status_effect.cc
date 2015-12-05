//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 19:44:05.
// File name: status_effect.cc
//
// Description:
// Define class StatusEffect.
//

#include "game_server/server/extension/effect/status_effect.h"

#include "global/logging.h"
#include "game_server/server/extension/effect/configure.h"
#include "game_server/server/extension/effect/effect_role.h"

namespace game {

namespace server {

namespace effect {

StatusEffect::StatusEffect() {}
StatusEffect::~StatusEffect() {}

bool StatusEffect::Initialize(EffectRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 初始化效果对象失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;

  this->forbid_move_ = 0;
  this->forbid_skill_ = 0;
  this->forbid_item_ = 0;
  this->forbid_beaten_map_ = 0;

  return true;
}

void StatusEffect::Finalize() {
  this->role_ = NULL;
}

bool StatusEffect::StartCommonStatus(EffectStatusType::type type) {
  this->SetStatus(type,true);
  return true;
}

bool StatusEffect::EndCommonStatus(EffectStatusType::type type) {
  this->SetStatus(type,false);
  return true;
}

void StatusEffect::SetStatus(EffectStatusType::type type, bool add) {
  if(this->role_ == NULL) {
    return ;
  }

  GameRole *role = this->role_->GetRole();
  if(role == NULL) {
    return ;
  }
  if(add != true) {
    switch(type) {
      case EffectStatusType::SOUL:
        role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
            role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) &
            ~entity::RoleCommonStatus::SOUL);
        break;
      case EffectStatusType::FORBID_MOVE:
        --this->forbid_move_;
        if(this->forbid_move_ <= 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) &
              ~entity::RoleCommonStatus::FORBID_MOVE);
          this->forbid_move_ = 0;
        }
        break;
      case EffectStatusType::FORBID_USE_SKILL:
        --this->forbid_skill_;
        if(this->forbid_skill_ <= 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) &
              ~entity::RoleCommonStatus::FORBID_USE_SKILL);
          this->forbid_skill_ = 0;
        }
        break;
      case EffectStatusType::FORBID_USE_ITEM:
        --this->forbid_item_;
        if(this->forbid_item_ <= 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) &
              ~entity::RoleCommonStatus::FORBID_USE_ITEM);
          this->forbid_item_ = 0;
        }
        break;
      case EffectStatusType::FORBID_BEATEN_MAP:
        --this->forbid_beaten_map_;
        if(this->forbid_beaten_map_ <= 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) &
              ~entity::RoleCommonStatus::FORBID_BEATEN_MAP);
          this->forbid_beaten_map_ = 0;
        }
        break;
      default:
        break;
    }
  } else {
    switch(type) {
      case EffectStatusType::SOUL:
        role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
            role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) |
            entity::RoleCommonStatus::SOUL);
        break;
      case EffectStatusType::FORBID_MOVE:
        if(this->forbid_move_ == 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) |
              entity::RoleCommonStatus::FORBID_MOVE);
        }
        ++this->forbid_move_;
        break;
      case EffectStatusType::FORBID_USE_SKILL:
        if(this->forbid_skill_ == 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) |
              entity::RoleCommonStatus::FORBID_USE_SKILL);
        }
        ++this->forbid_skill_;
        break;
      case EffectStatusType::FORBID_USE_ITEM:
        if(this->forbid_item_ == 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) |
              entity::RoleCommonStatus::FORBID_USE_ITEM);
        }
        ++this->forbid_item_;
        break;
      case EffectStatusType::FORBID_BEATEN_MAP:
        if(this->forbid_beaten_map_ == 0) {
          role->SetAttribute(entity::RoleAoiFields::COMMON_STATUS,
              role->GetAttribute(entity::RoleAoiFields::COMMON_STATUS) |
              entity::RoleCommonStatus::FORBID_BEATEN_MAP);
        }
        ++this->forbid_beaten_map_;
        break;
      default:
        break;
    }
  }
}

EffectStatusType::type StatusEffect::ConvertType(
    entity::RoleCommonStatus::type type) {
  switch(type) {
    case entity::RoleCommonStatus::SOUL:
      return EffectStatusType::SOUL;
    case entity::RoleCommonStatus::FORBID_MOVE:
      return EffectStatusType::FORBID_MOVE;
    case entity::RoleCommonStatus::FORBID_USE_SKILL:
      return EffectStatusType::FORBID_USE_SKILL;
    case entity::RoleCommonStatus::FORBID_USE_ITEM:
      return EffectStatusType::FORBID_USE_ITEM;
    case entity::RoleCommonStatus::FORBID_BEATEN_MAP:
      return EffectStatusType::FORBID_BEATEN_MAP;
    default:
      break;
  }
  return EffectStatusType::MAX;
}

bool StatusEffect::Start(core::uint32 id) {
  if(this->role_ == NULL || id == 0) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  GameRole *role = this->role_->GetRole();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败，得到 GameRole 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  const EffectConfigure *configure = 
    Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 开启效果(%u)时，类型不是状态效果",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }
  this->SetStatus((EffectStatusType::type)configure->param2_, true);
  return true;
}

bool StatusEffect::End(core::uint32 id) {
  if(this->role_ == NULL || id == 0) {
    global::LogError("%s:%d (%s) 关闭效果(%u)失败，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  GameRole *role = this->role_->GetRole();
  if(role == NULL) {
    global::LogError("%s:%d (%s) 关闭效果(%u)失败，得到 GameRole 失败");
    return false;
  }

  const EffectConfigure *configure = 
    Configure::GetInstance()->GetEffectConfigure(id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 关闭效果(%u)时，不在配置文件中",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  if(configure->type_ != this->GetType()) {
    global::LogError("%s:%d (%s) 关闭效果(%u)时，类型不是状态效果",
        __FILE__, __LINE__, __FUNCTION__, id);
    return false;
  }

  this->SetStatus((EffectStatusType::type)configure->param2_, false);
  return true;
}

}  // namespace effect

}  // namespace server

}  // namespace game

