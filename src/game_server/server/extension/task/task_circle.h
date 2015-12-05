//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-14 20:08:36.
// File name: task_circle.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__TASK__CIRCLE__H
#define __GAME__SERVER__TASK__TASK__CIRCLE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/storage_task_actor_login_types.h"
#include "database_server/protocol/storage_task_save_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace task {

class Task;
class TaskActor;

class TaskCircle {
  typedef database::protocol::StorageTaskActorField TaskActorFieldResult;

 public:
  TaskCircle();
  ~TaskCircle();

  bool Initialize(TaskActor *actor,  TaskActorFieldResult& taskData);
  bool InitializeTask(const database::protocol::StorageTaskField &field);

  void Finalize();

  void Save(database::protocol::StorageTaskActorField &field);
  void SaveTask(database::protocol::StorageTaskSaveRequest &request);

  core::int32 GetLvStage() const {
    return this->current_lv_stage_;
  }

  core::int32 GetFlag() const {
    return this->flag_;
  }

  core::int32 GetConditionID() const {
    return this->current_condition_id_;
  }

  core::int32 GetCurrentCircle() const {
    return this->current_circle_;
  }

  bool CheckCircleFinish() const;

  bool CheckRoundFinish() const;

  core::int32 GetCurrentRound() const;

  core::int32 GetCurrentTotalRound() const;

  // 同步客户端当前的循环任务信息
  void RoundSynchronize();
  // 打开一轮循环任务(如果当前的状态不是无效的，则失败)
  bool OpenRound();
  // 关闭当前的轮任务
  void CloseRound();
  // 关闭循环任务(当所有轮都完成并且奖励领取后，应当关闭)
  bool CloseTaskCircle();
  // 打开循环任务(当有循环任务可做)
  bool OpenTaskCircle();
  // 随机循环任务条件，结果受等级段影响, 返回 -1 失败
  core::int32 RandConditon();
  // 替换任务,受状态影响
  bool ReplaceTask();
  // 进行下一轮循环任务，改变循环任务当前环数的值,直到 20
  bool NextCircle();
  // 初始化循环任务,新玩家、下一天在线或者上线调用，num 表示增加轮数目
  void InitCircle(core::int32 num, bool new_role = true);
  // 任务处理
  void OnEvent(event::EventType::type type, const void *data, size_t size);

  bool CheckGather(core::int32 template_id);

  bool CheckSummonNpc(core::int32 template_id);

  Task *GetTask() {
    return this->task_;
  }

 private:
  // 设置等级段
  void SetLvStage();
  // 创建任务(条件)
  Task *CreateTask(core::int32 conditon);

  // 循环任务指针
  Task* task_;
  // 玩家任务对象
  TaskActor *actor_;
  // 当前进行的循环任务轮数目
  core::int32 current_round_;
  // 总共的循环任务轮数目
  core::int32 total_round_;
  // 当前轮任务进行到了多少
  core::int32 current_circle_;
  // 当前的等级段
  core::int32 current_lv_stage_;
  // 当前的任务条件ID
  core::int32 current_condition_id_;
  // 轮任务状态
  entity::TaskCircleRoundState::type flag_;
};

} // namespace task

} // namespace server

} // namespace game

#endif // __GAME__SERVER__TASK__TASK__CIRCLE__H
