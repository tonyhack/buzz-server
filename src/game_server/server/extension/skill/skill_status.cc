//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 14:42:40.
// File name: skill_status.cc
//
// Description:
// Define class SkillStatus.
//

#include "game_server/server/extension/skill/skill_status.h"

#include "game_server/server/extension/skill/skill_role.h"

namespace game {

namespace server {

namespace skill {

SkillStatus::SkillStatus() : role_(NULL) {}
SkillStatus::~SkillStatus() {}

bool SkillStatus::Initialize(SkillRole *role) {
  if(role == NULL) {
    global::LogError("%s:%d (%s) 初始化 SkillRole 失败, role为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->role_ = role;

  return true;
}

void SkillStatus::Finalize() {
  this->role_ = NULL;
}

}  // namespace skill

}  // namespace server

}  // namespace game

