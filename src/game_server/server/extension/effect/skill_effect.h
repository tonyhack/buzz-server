//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-31 16:29:36.
// File name: skill_effect.h
//
// Description:
// Define class SkillEffect.
//

#ifndef __GAME__SERVER__EFFECT__SKILL__EFFECT__H
#define __GAME__SERVER__EFFECT__SKILL__EFFECT__H

#include "core/base/types.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/effect_type.h"
#include "entity/role_types.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class SkillEffect : public Effect {
 public: 
  SkillEffect();
  virtual ~SkillEffect();

  virtual bool Initialize(EffectRole *role);
  virtual void Finalize();

  virtual bool Start(core::uint32 id);
  virtual bool End(core::uint32 id);

  virtual EffectType::type GetType() const {
    return EffectType::EFFECT_SKILL;
  }
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__SKILL__EFFECT__H

