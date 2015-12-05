//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 11:13:56.
// File name: scene_npc_pool.cc
//
// Description:
// Define class SceneNpcPool.
//

#include "game_server/server/extension/scene/scene_npc_pool.h"

#include "game_server/server/extension/scene/scene_npc.h"

namespace game {

namespace server {

namespace scene {

bool SceneNpcPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->npcs_.Initialize(initial_number, extend_number);
}
void SceneNpcPool::Finalize() {
  this->npcs_.Finalize();
}

SceneNpc *SceneNpcPool::Allocate() {
  return this->npcs_.Allocate();
}

void SceneNpcPool::Deallocate(SceneNpc *npc) {
  this->npcs_.Deallocate(npc);
}

}  // namespace scene

}  // namespace server

}  // namespace game

