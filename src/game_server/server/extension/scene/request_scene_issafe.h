//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-19 10:36:56.
// File name: request_scene_issafe.h
//
// Description:
// Define RequestSceneIsSafe.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__SCENE__ISSAFE__H
#define __GAME__SERVER__SCENE__REQUEST__SCENE__ISSAFE__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestSceneIsSafe : public core::Noncopyable {
 public:
  RequestSceneIsSafe();
  ~RequestSceneIsSafe();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__SCENE__ISSAFE__H

