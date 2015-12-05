//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 15:34:52.
// File name: request_use_skill.h
//
// Description:
// Define class RequestUseSkill.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__USE__SKILL__H
#define __GAME__SERVER__SKILL__REQUEST__USE__SKILL__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestUseSkill : public core::Noncopyable {
 public:
  RequestUseSkill();
  ~RequestUseSkill();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__USE__SKILL__H

