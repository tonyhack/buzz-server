//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:32:53.
// File name: task_builder.h
//
// Description:
// Define class TaskBuilder.
//

#ifndef __GAME__SERVER__TASK__TASK__BUILDER__H
#define __GAME__SERVER__TASK__TASK__BUILDER__H

#include "core/base/types.h"
#include "database_server/protocol/storage_task_login_types.h"
#include "entity/task_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace task {

class Task;
class TaskActor;

class TaskBuilder : public global::SingletonFactory<TaskBuilder> {
  friend class global::SingletonFactory<TaskBuilder>;

 public:
  bool Initialize();
  void Finalize();

  // 创建空的任务对象
  Task *Create(TaskActor *actor, entity::TaskType::type type, core::uint32 id);
  // 创建任务对象(初始任务条件变量数据)
  Task *Create(TaskActor *actor, entity::TaskType::type type, const database::protocol::StorageTaskField &field);

  void Destory(Task *task);

 private:
  TaskBuilder() {}
  ~TaskBuilder() {}
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__BUILDER__H

