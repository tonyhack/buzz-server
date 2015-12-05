//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 18:25:46.
// File name: task_actor_manager.h
//
// Description:
// Define class TaskActorManager.
//

#ifndef __GAME__SERVER__TASK__TASK__ACTOR__MANAGER__H
#define __GAME__SERVER__TASK__TASK__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class TaskActor;

class TaskActorManager : public global::SingletonFactory<TaskActorManager> {
  friend class global::SingletonFactory<TaskActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, TaskActor *> ActorHashmap;

 public:
  TaskActor *Get(core::uint64 id);
  bool Add(TaskActor *actor);
  TaskActor *Remove(core::uint64 id);
  
  void DailyClean(core::int32 days);
 private:
  TaskActorManager() {}
  ~TaskActorManager() {}

  ActorHashmap actors_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__ACTOR__MANAGER__H

