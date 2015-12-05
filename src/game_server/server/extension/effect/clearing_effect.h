//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-20 12:19:54.
// File name: clearing_effect.h
//
// Description:
// Define class ClearingEffect.
//

#ifndef __GAME__SERVER__EFFECT__CLEARING__EFFECT__H
#define __GAME__SERVER__EFFECT__CLEARING__EFFECT__H

#include "core/base/types.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/effect_type.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class ClearingEffect : public Effect {
 public:
  ClearingEffect();
  virtual ~ClearingEffect();

  virtual bool Initialize(EffectRole *role);
  virtual void Finalize();

  virtual bool Start(core::uint32 id);
  virtual bool End(core::uint32 id);

  virtual EffectType::type GetType() const {
    return EffectType::EFFECT_CLEARING;
  }
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__CLEARING__EFFECT__H

