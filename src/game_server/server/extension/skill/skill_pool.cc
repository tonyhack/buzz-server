//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-31 16:27:52.
// File name: skill_pool.cc
//
// Description:
// Define class SkillPool.
//

#include "game_server/server/extension/skill/skill_pool.h"

#include "game_server/server/extension/skill/skill.h"

namespace game {

namespace server {

namespace skill {

bool SkillPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->skills_.Initialize(initial_number, extend_number);
}

void SkillPool::Finalize() {
  this->skills_.Finalize();
}

Skill *SkillPool::Allocate() {
  return this->skills_.Allocate();
}

void SkillPool::Deallocate(Skill *skill) {
  this->skills_.Deallocate(skill);
}

}  // namespace skill

}  // namespace server

}  // namespace game

