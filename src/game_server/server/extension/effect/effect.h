//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 02:03:59.
// File name: effect.h
//
// Description:
// Define class Effect.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__H
#define __GAME__SERVER__EFFECT__EFFECT__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/effect/effect_type.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class Effect : public core::Noncopyable {
 public:
  Effect();
  virtual ~Effect();

  virtual bool Initialize(EffectRole *role) = 0;
  virtual void Finalize() = 0;

  virtual bool Start(core::uint32 id) = 0;
  virtual bool End(core::uint32 id) = 0;

  virtual EffectType::type GetType() const = 0;

 protected:
  EffectRole *role_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__H

