//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 16:10:52.
// File name: session_actor_pool.cc
//
// Description:
// Define class SessionActorPool.
//

#include "session_server/server/session_actor_pool.h"

#include "session_server/server/session_actor.h"

namespace session {

namespace server {

bool SessionActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void SessionActorPool::Finalize() {
  this->actors_.Finalize();
}

SessionActor *SessionActorPool::Allocate() {
  SessionActor *actor = this->actors_.Allocate();
  if(actor) {
    actor->Initialize();
  }
  return actor;
}

void SessionActorPool::Deallocate(SessionActor *actor) {
  if(actor) {
    actor->Finalize();
    this->actors_.Deallocate(actor);
  }
}

}  // namespace server

}  // namespace session

