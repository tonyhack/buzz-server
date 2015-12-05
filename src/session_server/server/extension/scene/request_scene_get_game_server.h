//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 09:52:26.
// File name: request_scene_get_game_server.h
//
// Description:
// Define class RequestSceneGetGameServer.
//

#ifndef __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__GAME__SERVER__H
#define __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__GAME__SERVER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace scene {

class RequestSceneGetGameServer : public core::Noncopyable {
 public:
  RequestSceneGetGameServer();
  ~RequestSceneGetGameServer();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__REQUEST__SCENE__GET__GAME__SERVER__H

