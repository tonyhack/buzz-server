//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-12 18:38:24.
// File name: scene_item_pool.cc
//
// Description:
// Define class SceneItemPool.
//

#include "game_server/server/extension/scene/scene_item_pool.h"

#include "game_server/server/extension/scene/scene_item.h"

namespace game {

namespace server {

namespace scene {

bool SceneItemPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->items_.Initialize(initial_number, extend_number);
}

void SceneItemPool::Finalize() {
  this->items_.Finalize();
}

SceneItem *SceneItemPool::Allocate() {
  SceneItem *item = this->items_.Allocate();
  if(item) {
    item->Allocate(this->sequence_++);
  }
  return item;
}

void SceneItemPool::Deallocate(SceneItem *item) {
  if(item) {
    item->Deallocate();
    this->items_.Deallocate(item);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

