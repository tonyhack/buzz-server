//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 21:12:49.
// File name: transport_actor_pool.cc
//
// Description:
// Define class TransportActorPool.
//

#include "game_server/server/extension/transport/transport_actor_pool.h"

#include "game_server/server/extension/transport/transport_actor.h"

namespace game {

namespace server {

namespace transport {

bool TransportActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void TransportActorPool::Finalize() {
  this->actors_.Finalize();
}

TransportActor *TransportActorPool::Allocate() {
  return this->actors_.Allocate();
}

void TransportActorPool::Deallocate(TransportActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace transport

}  // namespace server

}  // namespace game

