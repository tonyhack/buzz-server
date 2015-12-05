//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-10 11:36:58.
// File name: task_builder.cc
//
// Description:
// Define class TaskBuilder.
//

#include "game_server/server/extension/task/task_builder.h"

#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/load_circle_condition_configure.h"
#include "game_server/server/extension/task/task_pool.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

bool TaskBuilder::Initialize() {
  return true;
}

void TaskBuilder::Finalize() {}

Task *TaskBuilder::Create(TaskActor *actor, entity::TaskType::type type, core::uint32 id) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 创建任务(%u)时，参数错误",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }
  // 创建任务对象
  Task *task = TaskPool::GetInstance()->Allocate();
  if(task == NULL) {
    global::LogError("%s:%d (%s) 创建任务(%u)时，分配任务对象失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  // 初始化任务对象
  if(task->Initialize(actor, id, type, database::protocol::StorageTaskField()) == false) {
    TaskPool::GetInstance()->Deallocate(task);
    global::LogError("%s:%d (%s) 创建任务(%u)时，初始化任务对象失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  return task;
}

Task *TaskBuilder::Create(TaskActor *actor,entity::TaskType::type type,
    const database::protocol::StorageTaskField &field) {
  Task *task = TaskPool::GetInstance()->Allocate();
  if(task == NULL) {
    LOG_ERROR("任务申请内存失败");
    return NULL;
  }
  core::uint32 task_id = field.task_;
  if(type == entity::TaskType::TASK_CIRCLE) {
    task_id = actor->GetCircle().GetConditionID();
  }
  if(task->Initialize(actor, task_id, type, field) == false) {
    LOG_ERROR("任务[%d]初始化失败", task_id);
    TaskPool::GetInstance()->Deallocate(task);
    return NULL;
  }

  return task;
}

void TaskBuilder::Destory(Task *task) {
  if(task == NULL) {
    return ;
  }

  task->Finalize();

  TaskPool::GetInstance()->Deallocate(task);
}

}  // namespace task

}  // namespace server

}  // namespace game

