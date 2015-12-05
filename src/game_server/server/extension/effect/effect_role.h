//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 19:29:06.
// File name: effect_role.h
//
// Description:
// Define class EffectRole.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__ROLE__H
#define __GAME__SERVER__EFFECT__EFFECT__ROLE__H

#include <map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/effect/creator_key.h"
#include "game_server/server/extension/effect/effect_type.h"

namespace game {

namespace server {

namespace effect {

class Effect;

class EffectRole : public core::Noncopyable {
 public:
  EffectRole();
  ~EffectRole();

  bool Initialize(GameRole *role);
  void Finalize();

  // 开启/结束 某个效果
  bool StartEffect(core::uint32 effect);
  bool EndEffect(core::uint32 effect);

  // 设置/获取效果属性
  core::uint32 GetEffectAttribute(EffectAttributeType::type type);
  bool SetEffectAttribute(EffectAttributeType::type type, core::uint32 value);

  inline entity::EntityType::type GetType() const {
    if(this->role_) {
      return this->role_->GetType();
    }
    return entity::EntityType::TYPE_MAX;
  }

  inline core::uint64 GetID() const {
    if(this->role_) {
      return this->role_->GetID();
    }
    return 0;
  }

  inline Effect *GetEffect(EffectType::type type) {
    if(type >= EffectType::EFFECT_MIN || type < EffectType::EFFECT_MAX) {
      return this->effects_[type];
    }
    return NULL;
  }

  inline GameRole *GetRole() {
    return this->role_;
  }

 private:
  GameRole *role_;
  Effect *effects_[EffectType::EFFECT_MAX];
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__ROLE__H

