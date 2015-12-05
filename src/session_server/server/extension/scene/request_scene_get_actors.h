//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 10:57:10.
// File name: request_scene_get_actors.h
//
// Description:
// Define class RequestSceneGetActors.
//

#ifndef __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__ACTORS__H
#define __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__ACTORS__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace scene {

class RequestSceneGetActors : public core::Noncopyable {
 public:
  RequestSceneGetActors();
  ~RequestSceneGetActors();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__ACTORS__H

