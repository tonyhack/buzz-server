
#ifndef __GAME__SERVER__TASK__TASK__DAILY__H
#define __GAME__SERVER__TASK__TASK__DAILY__H

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

class TaskDaily : public core::Noncopyable {
  typedef std::map<core::uint32, Task *> TaskMap;
  typedef __gnu_cxx::hash_set<core::uint32> TaskHashset;
  typedef std::vector<bool> ExploitAwards;

 public:
  typedef std::vector<database::protocol::StorageTaskField> TaskVector;
  typedef std::vector<core::int32> FinishTaskVector;

  TaskDaily();
  ~TaskDaily();

  bool Initialize(TaskActor *actor, database::protocol::StorageTaskActorField &data);

  void Finalize();

  void Syschronize();

  void DailyTaskInit();
  
  void Save(database::protocol::StorageTaskActorField &field);
  void SaveTask(database::protocol::StorageTaskSaveRequest&  request);
  // 增加、获取、删除任务
  void AddTask(Task *task);

  void AddFinishTask(core::int32 task) {
    this->finish_tasks_.insert(task);
  }

  // 某个任务ID是否已完成
  bool CheckTaskFinish(core::uint32 task);
  
  // 检测任务能不能被接取
  bool CheckTaskAccept(const core::uint32 task);

  // 完成任务
  bool FinishTask(core::int32 id);

  // 检查奖励是否可领取
  bool CheckDailyAward(core::int32 id);

  // 重置已经领取
  void SetPicked(core::int32 picked);

  // 获取临时军功资源
  core::int32 GetExploit() const {
    return this->daily_award_exploit_;
  }

  // 恢复奖励
  void OnExploitEvent(core::int32 num);

  // 事件
  void OnEvent(event::EventType::type type, const void *data, size_t size);

  bool CheckGather(core::int32 npc_template);

  bool CheckSummonNpc(core::int32 npc_template);

  // 计算属性加成
  void SumAttr(core::int32 honour, bool add_des, bool sysnc);

  void DailyClean(core::int32 days = 0);

 private:
  // 玩家对象指针
  TaskActor *actor_;

  // 当前正在进行的任务
  TaskMap tasks_;

  // 已完成的任务
  TaskHashset finish_tasks_;

  // 军功奖励
  ExploitAwards excploit_awards_;

  // 每天军功奖励池
  core::int32 daily_award_exploit_;

};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__DAILY__H

