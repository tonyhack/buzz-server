//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 14:09:35.
// File name: request_scene_jump.h
//
// Description:
// Define class RequestSceneJump.
//

#ifndef __SESSION__SERVER__SCENE__REQUEST__SCENE__JUMP__H
#define __SESSION__SERVER__SCENE__REQUEST__SCENE__JUMP__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

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

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__REQUEST__SCENE__JUMP__H

