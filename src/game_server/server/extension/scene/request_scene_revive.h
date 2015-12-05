//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-16 16:39:46.
// File name: request_scene_revive.h
//
// Description:
// Define class RequestSceneRevive.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__REVIVE__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__REVIVE__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneRevive : public core::Noncopyable {
 public:
  RequestSceneRevive();
  ~RequestSceneRevive();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__REVIVE__H

