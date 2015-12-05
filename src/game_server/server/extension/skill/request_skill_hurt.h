//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-19 18:49:25.
// File name: request_skill_hurt.h
//
// Description:
// Define class RequestSkillHurt.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__SKILL__HURT__H
#define __GAME__SERVER__SKILL__REQUEST__SKILL__HURT__H

#include <stddef.h>
#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestSkillHurt : public core::Noncopyable {
 public:
  RequestSkillHurt();
  ~RequestSkillHurt();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__SKILL__HURT__H

