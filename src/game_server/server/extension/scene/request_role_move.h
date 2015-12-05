//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 13:53:56.
// File name: request_role_move.h
//
// Description:
// Define RequestRoleMove.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__ROLE__MOVE__H
#define __GAME__SERVER__SCENE__REQUEST__ROLE__MOVE__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestRoleMove : public core::Noncopyable {
 public:
  RequestRoleMove();
  ~RequestRoleMove();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__ROLE__MOVE__H

