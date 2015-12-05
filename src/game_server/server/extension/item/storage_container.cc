//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-24 11:29:55.
// File name: storage_container.cc
//
// Description:
// Define class StorageContainer.
//

#include "game_server/server/extension/item/storage_container.h"

#include "entity/actor_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/item/item_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

StorageContainer::StorageContainer() : load_finish_(false) {}
StorageContainer::~StorageContainer() {}

bool StorageContainer::Initialize(ItemActor *actor, core::uint16 capacity,
    core::uint16 max_capacity) {
  if(ItemContainer::Initialize(actor, capacity, max_capacity) == false) {
    global::LogError("%s:%d (%s) 初始化 ItemContainer 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->SetLoadFinish(false);

  return true;
}

void StorageContainer::Finalize() {
  ItemContainer::Finalize();
}

void StorageContainer::ExtendCapacity(core::int32 extend) {
  if(this->actor_ == NULL) {
    return ;
  }

  GameActor *actor = this->actor_->GetActor();
  if(actor == NULL) {
    return ;
  }

  actor->SetAttribute(entity::ActorClientFields::STORAGE_CAPACITY,
      actor->GetAttribute(entity::ActorClientFields::STORAGE_CAPACITY) + extend);

  this->capacity_ = actor->GetAttribute(entity::ActorClientFields::PACKET_CAPACITY);

  this->items_.resize(this->capacity_);
}

}  // namespace item

}  // namespace server

}  // namespace game

