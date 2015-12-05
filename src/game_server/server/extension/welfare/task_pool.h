
#ifndef __GAME__SERVER__WELFARE__TASK__POOL__H
#define __GAME__SERVER__WELFARE__TASK__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace welfare {

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

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__TASK__POOL__H

