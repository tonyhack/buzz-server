//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-27 22:59:43.
// File name: request_get_around_actors.h
//
// Description:
// Define class RequestGetAroundActors.
//

#ifndef __GAME__SERVER__SCENE__REQUEST__GET__AROUND__ACTORS__H
#define __GAME__SERVER__SCENE__REQUEST__GET__AROUND__ACTORS__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class RequestGetAroundActors : public core::Noncopyable {
 public:
  RequestGetAroundActors();
  ~RequestGetAroundActors();

  bool Initialize();
  void Finalize();

 private:
  int OnRequest(void *message, size_t size);

  static const size_t kMaxActorNumber_ = 2000;
  core::uint64 actors_[kMaxActorNumber_];
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__REQUEST__GET__AROUND__ACTORS__H

