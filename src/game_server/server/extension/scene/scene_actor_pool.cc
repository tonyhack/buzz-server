//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-12 11:15:53.
// File name: scene_actor_pool.cc
//
// Description:
// Define class SceneActorPool.
//

#include "game_server/server/extension/scene/scene_actor_pool.h"

#include "game_server/server/extension/scene/scene_actor.h"

namespace game {

namespace server {

namespace scene {

bool SceneActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}
void SceneActorPool::Finalize() {
  this->actors_.Finalize();
}

SceneActor *SceneActorPool::Allocate() {
  return this->actors_.Allocate();
}

void SceneActorPool::Deallocate(SceneActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace scene

}  // namespace server

}  // namespace game

