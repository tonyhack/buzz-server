//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-24 14:55:01.
// File name: request_stand_skill.h
//
// Description:
// Define class RequestStandSkill.
//

#ifndef __GAME__SERVER__SKILL__REQUEST__STAND__SKILL__H
#define __GAME__SERVER__SKILL__REQUEST__STAND__SKILL__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace skill {

class RequestStandSkill : public core::Noncopyable {
 public:
  RequestStandSkill();
  ~RequestStandSkill();

  bool Initialize();
  void Finalize();

 private:
  int OnRequestStandSkill(void *message, size_t size);
  int OnRequestSitSkill(void *message, size_t size);
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__REQUEST__STAND__SKILL__H

