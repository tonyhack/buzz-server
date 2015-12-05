//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-27 13:38:10.
// File name: request_actor_broadcast.h
//
// Description:
// Define class RequestActorBroadcast.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__ACTOR__BROADCAST__H
#define __GAME__SERVER__SCENE__REQUEST__ACTOR__BROADCAST__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestActorBroadcast : public core::Noncopyable {
 public:
  RequestActorBroadcast();
  ~RequestActorBroadcast();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__ACTOR__BROADCAST__H

