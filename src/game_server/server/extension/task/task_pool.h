//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:39:19.
// File name: task_pool.h
//
// Description:
// Define class TaskPool.
//

#ifndef __GAME__SERVER__TASK__TASK__POOL__H
#define __GAME__SERVER__TASK__TASK__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class Task;

class TaskPool : public global::SingletonFactory<TaskPool> {
  friend class global::SingletonFactory<TaskPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Task *Allocate();
  void Deallocate(Task *task);

 private:
  TaskPool() {}
  ~TaskPool() {}

  global::PoolTemplate<Task> tasks_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__POOL__H

