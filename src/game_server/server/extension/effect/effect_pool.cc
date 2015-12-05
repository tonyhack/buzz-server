//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 22:46:36.
// File name: effect_pool.cc
//
// Description:
// Define class EffectPool.
//

#include "game_server/server/extension/effect/effect_pool.h"

#include "game_server/server/extension/effect/attribute_effect.h"
#include "game_server/server/extension/effect/clearing_effect.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/skill_effect.h"
#include "game_server/server/extension/effect/status_effect.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace effect {

bool EffectPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->attribute_effect_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化属性效果失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->clearing_effect_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化清除效果失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->status_effect_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化状态效果失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if (this->skill_effect_.Initialize(initial_number, extend_number) == false) {
    LOG_ERROR("初始化技能效果失败");
    return false;
  }

  return true;
}

void EffectPool::Finalize() {
  this->attribute_effect_.Finalize();
  this->clearing_effect_.Finalize();
  this->status_effect_.Finalize();
  this->skill_effect_.Finalize();
}

Effect *EffectPool::Allocate(EffectType::type type) {
  if(type < EffectType::EFFECT_MIN || type >= EffectType::EFFECT_MAX) {
    return NULL;
  }

  switch(type) {
    case EffectType::EFFECT_ATTRIBUTE:
      return this->attribute_effect_.Allocate();
      break;
    case EffectType::EFFECT_CLEARING:
      return this->clearing_effect_.Allocate();
      break;
    case EffectType::EFFECT_STATUS:
      return this->status_effect_.Allocate();
      break;
    case EffectType::EFFECT_SKILL:
      return this->skill_effect_.Allocate();
    default:
      break;
  }

  return NULL;
}

void EffectPool::Deallocate(Effect *effect) {
  if(effect == NULL) {
    global::LogError("%s:%d (%s) 参数 effect 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(effect->GetType()) {
    case EffectType::EFFECT_ATTRIBUTE:
      this->attribute_effect_.Deallocate((AttributeEffect *)effect);
      break;
    case EffectType::EFFECT_CLEARING:
      this->clearing_effect_.Deallocate((ClearingEffect *)effect);
      break;
    case EffectType::EFFECT_STATUS:
      this->status_effect_.Deallocate((StatusEffect *)effect);
      break;
    case EffectType::EFFECT_SKILL:
      this->skill_effect_.Deallocate((SkillEffect *)effect);
      break;
    default:
      global::LogError("%s:%d (%s) 找不到效果(%d)对象池",
          __FILE__, __LINE__, __FUNCTION__, effect->GetType());
      break;
  }
}

}  // namespace effect

}  // namespace server

}  // namespace game

