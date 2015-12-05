//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 22:27:54.
// File name: effect_pool.h
//
// Description:
// Define class EffectPool.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__POOL__H
#define __GAME__SERVER__EFFECT__EFFECT__POOL__H

#include "game_server/server/extension/effect/effect_type.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace effect {

class Effect;
class AttributeEffect;
class ClearingEffect;
class StatusEffect;
class SkillEffect;

class EffectPool : public global::SingletonFactory<EffectPool> {
  friend class global::SingletonFactory<EffectPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Effect *Allocate(EffectType::type type);
  void Deallocate(Effect *effect);

 private:
  EffectPool() {}
  ~EffectPool() {}

  global::PoolTemplate<AttributeEffect> attribute_effect_;
  global::PoolTemplate<ClearingEffect> clearing_effect_;
  global::PoolTemplate<StatusEffect> status_effect_;
  global::PoolTemplate<SkillEffect> skill_effect_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__POOL__H

