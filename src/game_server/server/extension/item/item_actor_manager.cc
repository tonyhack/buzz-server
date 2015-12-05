//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-20 10:32:59.
// File name: item_actor_manager.cc
//
// Description:
// Define class ItemActorManager.
//

#include "game_server/server/extension/item/item_actor_manager.h"

#include "game_server/server/extension/item/item_actor.h"

namespace game {

namespace server {

namespace item {

ItemActor *ItemActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool ItemActorManager::Add(ItemActor *actor) {
  // 检测指针是否合法
  if(actor == NULL) {
    return false;
  }
  // 检测是否存在
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  // 加入管理器中
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

ItemActor *ItemActorManager::Remove(core::uint64 id) {
  ItemActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

void ItemActorManager::DailyClean(core::int32 days) {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    ItemActor *actor = iterator->second;
    if(actor) {
      actor->DailyClean(days);
    }
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

