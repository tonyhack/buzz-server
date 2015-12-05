#include "game_server/server/extension/attr/attr_actor_pool.h"

namespace game {

namespace server {

namespace attr {

bool AttrActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void AttrActorPool::Finalize() {
  this->actors_.Finalize();
}

AttrActor *AttrActorPool::Allocate() {
  return this->actors_.Allocate();
}

void AttrActorPool::Deallocate(AttrActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace attr

}  // namespace server

}  // namespace game

