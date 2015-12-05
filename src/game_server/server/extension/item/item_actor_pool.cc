//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:24:17.
// File name: item_actor_pool.cc
//
// Description:
// Define class ItemActorPool.
//

#include "game_server/server/extension/item/item_actor_pool.h"

#include "game_server/server/extension/item/item_actor.h"

namespace game {

namespace server {

namespace item {

bool ItemActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void ItemActorPool::Finalize() {
  this->actors_.Finalize();
}

ItemActor *ItemActorPool::Allocate() {
  return this->actors_.Allocate();
}

void ItemActorPool::Deallocate(ItemActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace item

}  // namespace server

}  // namespace game

