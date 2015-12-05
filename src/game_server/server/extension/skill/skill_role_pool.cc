//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:39:12.
// File name: skill_role_pool.cc
//
// Description:
// Define class SkillRolePool.
//

#include "game_server/server/extension/skill/skill_role_pool.h"

#include "game_server/server/extension/skill/skill_role.h"

namespace game {

namespace server {

namespace skill {

bool SkillRolePool::Initialize(size_t initial_number, size_t extend_number) {
  return this->roles_.Initialize(initial_number, extend_number);
}

void SkillRolePool::Finalize() {
  this->roles_.Finalize();
}

SkillRole *SkillRolePool::Allocate() {
  return this->roles_.Allocate();
}

void SkillRolePool::Deallocate(SkillRole *role) {
  this->roles_.Deallocate(role);
}

}  // namespace skill

}  // namespace server

}  // namespace game

