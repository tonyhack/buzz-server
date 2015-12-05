//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 15:36:06.
// File name: soul_actor_pool.cc
//
// Description:
// Define class SoulActorPool.
//

#include "game_server/server/extension/soul/soul_actor_pool.h"

#include "game_server/server/extension/soul/soul_actor.h"

namespace game {

namespace server {

namespace soul {

bool SoulActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void SoulActorPool::Finalize() {
  this->actors_.Finalize();
}

SoulActor *SoulActorPool::Allocate() {
  return this->actors_.Allocate();
}

void SoulActorPool::Deallocate(SoulActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace soul

}  // namespace server

}  // namespace game

