//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-07 11:14:47.
// File name: request_skill_alter.h
//
// Description:
// Define class RequestSkillAlter.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__SKILL__ALTER__H
#define __GAME__SERVER__SKILL__REQUEST__SKILL__ALTER__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestSkillAlter : public core::Noncopyable {
 public:
  RequestSkillAlter();
  ~RequestSkillAlter();

  bool Initialize();
  void Finalize();

 private:
  // 增加技能
  int OnRequestAddSkill(void *message, size_t size);
  // 删除技能
  int OnRequestRemoveSkill(void *message, size_t size);
  // 升级技能
  int OnRequestUpgradeSkill(void *message, size_t size);
  // 改变技能属性
  int OnRequestSkillIncreaseAttribute(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__SKILL__ALTER__H

