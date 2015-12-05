//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:26:13.
// File name: effect_role_pool.cc
//
// Description:
// Define class EffectRolePool.
//

#include "game_server/server/extension/effect/effect_role_pool.h"

#include "game_server/server/extension/effect/effect_role.h"

namespace game {

namespace server {

namespace effect {

bool EffectRolePool::Initialize(size_t initial_number,
    size_t extend_number) {
  return this->roles_.Initialize(initial_number, extend_number);
}

void EffectRolePool::Finalize() {
  this->roles_.Finalize();
}

EffectRole *EffectRolePool::Allocate() {
  return this->roles_.Allocate();
}

void EffectRolePool::Deallocate(EffectRole *role) {
  this->roles_.Deallocate(role);
}

}  // namespace effect

}  // namespace server

}  // namespace game

