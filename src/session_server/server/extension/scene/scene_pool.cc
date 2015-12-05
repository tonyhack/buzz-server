//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-06 12:25:10.
// File name: scene_pool.cc
//
// Description:
// Define class ScenePool.
//

#include "session_server/server/extension/scene/scene_pool.h"

#include "session_server/server/extension/scene/scene.h"

namespace session {

namespace server {

namespace scene {

bool ScenePool::Initialize(size_t initial_number, size_t extend_number) {
  return this->scenes_.Initialize(initial_number, extend_number);
}

void ScenePool::Finalize() {
  this->scenes_.Finalize();
}

Scene *ScenePool::Allocate() {
  return this->scenes_.Allocate();
}

void ScenePool::Deallocate(Scene *scene) {
  this->scenes_.Deallocate(scene);
}

}  // namespace scene

}  // namespace server

}  // namespace session

