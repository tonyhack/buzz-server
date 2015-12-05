//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:42:08.
// File name: task_pool.cc
//
// Description:
// Define class TaskPool.
//

#include "game_server/server/extension/task/task_pool.h"

#include "game_server/server/extension/task/task.h"

namespace game {

namespace server {

namespace task {

bool TaskPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->tasks_.Initialize(initial_number, extend_number);
}

void TaskPool::Finalize() {
  this->tasks_.Finalize();
}

Task *TaskPool::Allocate() {
  return this->tasks_.Allocate();
}

void TaskPool::Deallocate(Task *task) {
  this->tasks_.Deallocate(task);
}

}  // namespace task

}  // namespace server

}  // namespace game

