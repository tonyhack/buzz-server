//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-13 11:42:35.
// File name: scene_item_manager.cc
//
// Description:
// Define class SceneItemManager.
//

#include "game_server/server/extension/scene/scene_item_manager.h"

#include "game_server/server/extension/scene/scene_item.h"

namespace game {

namespace server {

namespace scene {

SceneItem *SceneItemManager::Get(core::uint64 id) {
  ItemHashmap::iterator iterator = this->items_.find(id);
  if(iterator != this->items_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool SceneItemManager::Add(SceneItem *item) {
  if(item == NULL) {
    return false;
  }

  ItemHashmap::const_iterator iterator = this->items_.find(item->GetID());
  if(iterator != this->items_.end()) {
    return false;
  }
  this->items_.insert(std::make_pair(item->GetID(), item));

  return true;
}

SceneItem *SceneItemManager::Remove(core::uint64 id) {
  SceneItem *item = NULL;
  ItemHashmap::iterator iterator = this->items_.find(id);
  if(iterator != this->items_.end()) {
    item = iterator->second;
    this->items_.erase(iterator);
  }
  return item;
}

}  // namespace scene

}  // namespace server

}  // namespace game

