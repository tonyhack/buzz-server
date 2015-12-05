//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 18:41:36.
// File name: task_actor_pool.h
//
// Description:
// Define class TaskActorPool.
//

#ifndef __GAME__SERVER__TASK__TASK__ACTOR__POOL__H
#define __GAME__SERVER__TASK__TASK__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class TaskActor;

class TaskActorPool : public global::SingletonFactory<TaskActorPool> {
  friend class global::SingletonFactory<TaskActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  TaskActor *Allocate();
  void Deallocate(TaskActor *actor);

 private:
  TaskActorPool() {}
  ~TaskActorPool() {}

  global::PoolTemplate<TaskActor> actors_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__ACTOR__POOL__H

