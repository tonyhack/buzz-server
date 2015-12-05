//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:10:18.
// File name: session_actor_pool.h
//
// Description:
// Define class SessionActorPool.
//

#ifndef __SESSION__SERVER__SESSION__ACTOR__POOL__H
#define __SESSION__SERVER__SESSION__ACTOR__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

class SessionActor;

class SessionActorPool : public global::SingletonFactory<SessionActorPool> {
  friend class global::SingletonFactory<SessionActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SessionActor *Allocate();
  void Deallocate(SessionActor *actor);

 private:
  SessionActorPool() {}
  ~SessionActorPool() {}

  global::PoolTemplate<SessionActor> actors_;
};

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SESSION__ACTOR__POOL__H

