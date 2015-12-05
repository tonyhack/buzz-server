//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-03 02:24:10.
// File name: attribute_effect.h
//
// Description:
// Define class AttributeEffect.
//

#ifndef __GAME__SERVER__EFFECT__ATTRIBUTE__EFFECT__H
#define __GAME__SERVER__EFFECT__ATTRIBUTE__EFFECT__H

#include "core/base/types.h"
#include "game_server/server/extension/effect/effect.h"
#include "game_server/server/extension/effect/effect_type.h"

namespace game {

namespace server {

namespace effect {

class AttributeEffect : public Effect {
 public:
  AttributeEffect();
  virtual ~AttributeEffect();

  virtual bool Initialize(EffectRole *role);
  virtual void Finalize();

  virtual bool Start(core::uint32 id);
  virtual bool End(core::uint32 id);

  virtual EffectType::type GetType() const {
    return EffectType::EFFECT_ATTRIBUTE;
  }
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__ATTRIBUTE__EFFECT__H

