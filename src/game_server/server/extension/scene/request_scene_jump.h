//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-13 11:48:26.
// File name: request_scene_jump.h
//
// Description:
// Define class RequestSceneJump.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__JUMP__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__JUMP__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneJump : public core::Noncopyable {
 public:
  RequestSceneJump();
  ~RequestSceneJump();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  //  namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__JUMP__H

