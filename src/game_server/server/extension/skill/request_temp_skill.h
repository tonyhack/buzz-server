//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-02-18 12:13:44.
// File name: request_temp_skill.h
//
// Description:
// Define class RequestTempSkill.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__TEMP__SKILL__H
#define __GAME__SERVER__SKILL__REQUEST__TEMP__SKILL__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestTempSkill : public core::Noncopyable {
 public:
  RequestTempSkill();
  ~RequestTempSkill();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestSkillAddTempSkill(void *message, size_t size);
  int OnRequestSkillRemoveTempSkill(void *message, size_t size);
  int OnRequestSkillChangeSkillForm(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__TEMP__SKILL__H

