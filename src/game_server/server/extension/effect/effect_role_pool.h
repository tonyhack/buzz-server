//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:24:40.
// File name: effect_role_pool.h
//
// Description:
// Define class EffectRolePool.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__ROLE__POOL__H
#define __GAME__SERVER__EFFECT__EFFECT__ROLE__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class EffectRolePool : public global::SingletonFactory<EffectRolePool> {
  friend class global::SingletonFactory<EffectRolePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  EffectRole *Allocate();
  void Deallocate(EffectRole *role);

 private:
  EffectRolePool() {}
  ~EffectRolePool() {}

  global::PoolTemplate<EffectRole> roles_;
};

}  // namespace effect

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__ROLE__POOL__H

