//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 17:37:55.
// File name: task.h
//
// Description:
// Define class Task.
//

#ifndef __GAME__SERVER__TASK__TASK__H
#define __GAME__SERVER__TASK__TASK__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_task_login_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_configure.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"

namespace game {

namespace server {

namespace task {

class Condition;
class TaskActor;

class Task : public core::Noncopyable {
 public:
  enum TaskCoordinateType {
    FINISH = 0,
    ACCEPT,
    CONDITION,
  };

  typedef std::vector<Condition *> ConditionVector;

  Task();
  ~Task();

  bool Initialize(TaskActor *actor, core::uint32 id,entity::TaskType::type type, 
      const database::protocol::StorageTaskField &field);
  void Finalize();

  void ExportDatabaseField(database::protocol::StorageTaskField &field);
  void ExportVariable(gateway::protocol::TaskFields &var);

  inline TaskActor *GetActor() {
    return this->actor_;
  }

  inline core::uint32 GetID() const {
    return this->id_;
  }

  // 增加条件
  bool AddCondition(Condition *condition);

  // 条件检测
  bool CheckCondition();

  // 检测招怪任务存在或者完成
  bool CheckSummonNpc(core::int32 npc_template);

  // 检测是不是采集任务，并且对应NPC
  bool CheckGather(core::int32 npc_template = 0);

  // 事件回调
  void OnEvent(event::EventType::type type, const void *data, size_t size);

  // 同步任务变量
  void SynchronizeVariable(const gateway::protocol::TaskVar &var);

  // 获取坐标
  bool GetCoordinate(Task::TaskCoordinateType type, core::int32 pos, core::int32 &out_map, core::int32 &out_x, core::int32 &out_y);

  void Clear();

  // 完成时调用
  void OnFinish();
  // 放弃时调用
  void OnAbandon();

 private:
  // 加入条件
  bool AddCondition(const TaskCondition &cell, 
      const database::protocol::StorageTaskField &field); 
  // 任务玩家对象
  TaskActor *actor_;

  // 任务ID
  core::uint32 id_;

  // 条件列表
  ConditionVector conditions_;

  // 任务类型
  entity::TaskType::type type_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__H

