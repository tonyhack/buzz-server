//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 23:35:30.
// File name: task_trip.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__TASK__TRIP__H
#define __GAME__SERVER__TASK__TASK__TRIP__H

#include <map>
#include <vector>

#include <ext/hash_set>

#include "core/base/noncopyable.h"
#include "database_server/protocol/storage_task_login_types.h"
#include "database_server/protocol/storage_task_actor_login_types.h"
#include "database_server/protocol/storage_task_save_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace task {

class Task;
class TaskActor;

class TaskTrip : public core::Noncopyable {
  typedef std::map<core::uint32, Task *> TaskMap;

 public:
  typedef std::vector<database::protocol::StorageTaskField> TaskVector;
  typedef std::vector<core::int32> FinishTaskVector;

  TaskTrip();
  ~TaskTrip();

  bool Initialize(TaskActor *actor);

  void Finalize();
  
  void Save(database::protocol::StorageTaskActorField &field);
  void SaveTask(database::protocol::StorageTaskSaveRequest&  request);
  // 增加、获取、删除任务
  bool AddTask(Task *task);
  Task *GetTask(core::uint32 id);
  bool RemoveTask(core::uint32 id);

  // 某个任务ID是否已完成
  bool CheckTaskFinish(core::uint32 task);
  
  // 检测任务能不能被接取
  bool CheckTaskAccept(const core::uint32 task);

  TaskActor *GetActor() const;

  // 事件
  void OnEvent(event::EventType::type type, const void *data, size_t size);

  core::int32 GetTaskNum() const {
    return this->task_num_;
  }

  void SetTaskNum(core::int32 num) {
    this->task_num_ = num;
  }
  core::int32 GetExecTaskNum() const {
    return this->tasks_.size();
  }
  void TripSynchoronize();

  bool CheckGather(core::int32 npc_template);

  bool CheckSummonNpc(core::int32 npc_Template);

 private:
  // 玩家对象指针
  TaskActor *actor_;

  // 当前正在进行的任务
  TaskMap tasks_;

  // 当天完成任务的数目
  core::int32 task_num_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__TRIP__H

