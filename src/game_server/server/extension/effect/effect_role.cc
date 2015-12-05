//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 21:43:35.
// File name: effect_role.cc
//
// Description:
// Define class EffectRole.
//

#include "game_server/server/extension/effect/effect_role.h"

#include "entity/entity_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/effect/configure.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/effect_pool.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

EffectRole::EffectRole() : role_(NULL) {}
EffectRole::~EffectRole() {}

bool EffectRole::Initialize(GameRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 参数 role 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  for(size_t pos = EffectType::EFFECT_MIN; pos < EffectType::EFFECT_MAX; ++pos) {
    Effect *effect = EffectPool::GetInstance()->Allocate((EffectType::type)pos);
    if(effect == NULL) {
      global::LogError("%s:%d (%s) 角色(%u,%u) 分配效果(%u) 对象失败",
          __FILE__, __LINE__, __FUNCTION__, role->GetType(),
          role->GetID(), pos);
      return false;
    }
    if(effect->Initialize(this) == false) {
      EffectPool::GetInstance()->Deallocate(effect);
      global::LogError("%s:%d (%s) 角色(%u,%u) 初始化效果(%u) 对象失败",
          __FILE__, __LINE__, __FUNCTION__, role->GetType(),
          role->GetID(), pos);
      return false;
    }
    this->effects_[pos] = effect;
  }

  this->role_ = role;

  return true;
}

void EffectRole::Finalize() {
  for(size_t pos = EffectType::EFFECT_MIN; pos < EffectType::EFFECT_MAX; ++pos) {
    if(this->effects_[pos]) {
      this->effects_[pos]->Finalize();
      EffectPool::GetInstance()->Deallocate(this->effects_[pos]);
    }
  }
  this->role_ = NULL;
}

bool EffectRole::StartEffect(core::uint32 effect_id) {
  // 获取配置信息
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(effect_id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 找不到效果(%u)配置", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  Effect *effect = this->GetEffect((EffectType::type)configure->type_);
  if(effect == NULL) {
    global::LogError("%s:%d (%s) 获取效果(%u)对象失败", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  if(effect->Start(effect_id) == false) {
    global::LogError("%s:%d (%s) 开启效果(%u)失败", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  return true;
}

bool EffectRole::EndEffect(core::uint32 effect_id) {
  // 获取配置信息
  const EffectConfigure *configure = Configure::GetInstance()->GetEffectConfigure(effect_id);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 找不到效果(%u)配置", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  Effect *effect = this->GetEffect((EffectType::type)configure->type_);
  if(effect == NULL) {
    global::LogError("%s:%d (%s) 获取效果(%u)对象失败", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  if(effect->End(effect_id) == false) {
    global::LogError("%s:%d (%s) 关闭效果(%u)失败", __FILE__, __LINE__,
        __FUNCTION__, effect_id);
    return false;
  }

  return true;
}

core::uint32 EffectRole::GetEffectAttribute(EffectAttributeType::type type) {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 获取效果属性时 role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return 0;
  }
  // 得到属性索引(不同类型的从0开始)
  core::uint32 attribute = type % kEffectAttributeTypeMask;
  // 得到属性类别
  core::uint32 entity_type = type - attribute;
  switch(entity_type) {
    case EffectAttributeType::ENTITY_CLIENT_BEGIN:
      return this->role_->GetAttribute((entity::EntityClientFields::type)attribute);
      break;
    case EffectAttributeType::ENTITY_SERVER_BEGIN:
      return this->role_->GetAttribute((entity::EntityServerFields::type)attribute);
      break;
    case EffectAttributeType::ROLE_AOI_BEGIN:
      return this->role_->GetAttribute((entity::RoleAoiFields::type)attribute);
      break;
    case EffectAttributeType::ROLE_CLIENT_BEGIN:
      return this->role_->GetAttribute((entity::RoleClientFields::type)attribute);
      break;
    case EffectAttributeType::ROLE_SERVER_BEGIN:
      return this->role_->GetAttribute((entity::RoleServerFields::type)attribute);
      break;
    case EffectAttributeType::ACTOR_AOI_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->GetAttribute(
            (entity::ActorAoiFields::type)attribute);
      }
      break;
    case EffectAttributeType::ACTOR_CLIENT_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->GetAttribute(
            (entity::ActorClientFields::type)attribute);
      }
      break;
    case EffectAttributeType::ACTOR_SERVER_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->GetAttribute(
            (entity::ActorServerFields::type)attribute);
      }
      break;
    default:
      break;
  }

  global::LogError("%s:%d (%s) 获取效果属性(%d)失败", __FILE__, __LINE__,
      __FUNCTION__, type);
  return 0;
}

bool EffectRole::SetEffectAttribute(EffectAttributeType::type type,
    core::uint32 value) {
  if(this->role_ == NULL) {
    global::LogError("%s:%d (%s) 设置效果属性时 role_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // 得到属性索引(不同类型的从0开始)
  core::uint32 attribute = type % kEffectAttributeTypeMask;
  // 得到属性类别
  core::uint32 entity_type = type - attribute;
  switch(entity_type) {
    case EffectAttributeType::ENTITY_CLIENT_BEGIN:
      return this->role_->SetAttribute((entity::EntityClientFields::type)attribute, value);
      break;
    case EffectAttributeType::ENTITY_SERVER_BEGIN:
      return this->role_->SetAttribute((entity::EntityServerFields::type)attribute, value);
      break;
    case EffectAttributeType::ROLE_AOI_BEGIN:
      return this->role_->SetAttribute((entity::RoleAoiFields::type)attribute, value);
      break;
    case EffectAttributeType::ROLE_CLIENT_BEGIN:
      return this->role_->SetAttribute((entity::RoleClientFields::type)attribute, value);
      break;
    case EffectAttributeType::ROLE_SERVER_BEGIN:
      return this->role_->SetAttribute((entity::RoleServerFields::type)attribute, value);
      break;
    case EffectAttributeType::ACTOR_AOI_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->SetAttribute(
            (entity::ActorAoiFields::type)attribute, value);
      }
      break;
    case EffectAttributeType::ACTOR_CLIENT_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->SetAttribute(
            (entity::ActorClientFields::type)attribute, value);
      }
      break;
    case EffectAttributeType::ACTOR_SERVER_BEGIN:
      if(this->role_->GetType() == entity::EntityType::TYPE_ACTOR) {
        return ((GameActor *)this->role_)->SetAttribute(
            (entity::ActorServerFields::type)attribute, value);
      }
      break;
    default:
      break;
  }

  global::LogError("%s:%d (%s) 设置效果属性(%d)失败", __FILE__, __LINE__,
      __FUNCTION__, type);
  return false;
}

}  // namespace effect

}  // namespace server

}  // namespace game

