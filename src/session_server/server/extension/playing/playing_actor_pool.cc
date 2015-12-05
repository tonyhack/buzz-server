//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 09:45:11.
// File name: playing_actor_pool.cc
//
// Description:
// Define class PlayingActorPool.
//

#include "session_server/server/extension/playing/playing_actor_pool.h"

#include "session_server/server/extension/playing/playing_actor.h"

namespace session {

namespace server {

namespace playing {

bool PlayingActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void PlayingActorPool::Finalize() {
  this->actors_.Finalize();
}

PlayingActor *PlayingActorPool::Allocate() {
  return this->actors_.Allocate();
}

void PlayingActorPool::Deallocate(PlayingActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace playing

}  // namespace server

}  // namespace session

