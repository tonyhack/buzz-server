//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 18:44:59.
// File name: task_actor_pool.cc
//
// Description:
// Define class TaskActorPool.
//

#include "game_server/server/extension/task/task_actor_pool.h"

#include "game_server/server/extension/task/task_actor.h"

namespace game {

namespace server {

namespace task {

bool TaskActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void TaskActorPool::Finalize() {
  this->actors_.Finalize();
}

TaskActor *TaskActorPool::Allocate() {
  return this->actors_.Allocate();
}

void TaskActorPool::Deallocate(TaskActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace task

}  // namespace server

}  // namespace game

