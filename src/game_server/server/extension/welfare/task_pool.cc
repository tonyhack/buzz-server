
#include "game_server/server/extension/welfare/task_pool.h"

#include "game_server/server/extension/welfare/task.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

bool TaskPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->tasks_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化 Task 对象池失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void TaskPool::Finalize() {
  this->tasks_.Finalize();
}

Task *TaskPool::Allocate() {
  return this->tasks_.Allocate();
}

void TaskPool::Deallocate(Task *task) {
  if(task == NULL) {
    return ;
  }
  this->tasks_.Deallocate(task);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
