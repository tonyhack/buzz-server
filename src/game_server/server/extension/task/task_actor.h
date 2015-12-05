//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-01 15:03:59.
// File name: task_actor.h
//
// Description:
// Define class TaskActor.
//

#ifndef __GAME__SERVER__TASK__TASK__ACTOR__H
#define __GAME__SERVER__TASK__TASK__ACTOR__H

#include <map>
#include <vector>

#include <ext/hash_set>

#include "core/base/noncopyable.h"
#include "database_server/protocol/storage_task_login_types.h"
#include "database_server/protocol/storage_task_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task_circle.h"
#include "game_server/server/extension/task/task_trip.h"
#include "game_server/server/extension/task/task_daily.h"
#include "game_server/server/extension/task/task_transport.h"
#include "game_server/server/event/game_event_types.h"

namespace game {

namespace server {

namespace task {

class Task;

class TaskActor : public core::Noncopyable {
  typedef __gnu_cxx::hash_set<core::uint32> TaskHashset;
  typedef std::map<core::uint32, Task *> TaskMap;
  typedef std::set<core::int32> LayerMap;

 public:
  typedef std::vector<database::protocol::StorageTaskField> TaskVector;
  typedef database::protocol::StorageTaskActorField TaskActorFieldResult;
  typedef std::vector<database::protocol::StorageTaskFinish> FinishTaskVector;

  TaskActor();
  ~TaskActor();

  bool InitializeTasks(const TaskVector &tasks,
      const FinishTaskVector &finishes);
  bool Initialize(GameActor *actor, TaskActorFieldResult& taskData);

  void Finalize();

  void Save();

  // 增加、获取、删除主线任务
  bool AddTask(Task *task);
  Task *AddTaskById(core::uint32 id);
  Task *GetTask(core::uint32 id);
  Task *RemoveTask(core::uint32 id);

  // 某个任务ID是否已完成
  inline bool CheckTaskFinish(core::uint32 task) const {
    if(task == 0) {
      return true;
    } else {
      return this->finish_tasks_.find(task) != this->finish_tasks_.end();
    }
  }
  inline bool FinishTask(core::uint32 task) {
    if(this->finish_tasks_.find(task) == this->finish_tasks_.end()) {
      this->finish_tasks_.insert(task);
      return true;
    }
    return false;
  }

  // 检查任务是否可接取
  bool CheckTaskAccept(core::int32 task);

  inline GameActor *GetActor() {
    return this->actor_;
  }

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return -1;
  }

  inline const std::string GetStringID() const {
    if(this->actor_) {
      return this->actor_->GetStringID();
    }
    return std::string();
  }
  
  inline TaskCircle& GetCircle() {
    return this->circle_task_;
  }
  inline TaskTrip& GetTrip() {
    return this->trip_task_;
  }
  inline TaskDaily& GetTaskDaily() {
    return this->task_daily_;
  }

  inline TaskTransport &GetTaskTransport() {
    return this->task_transport_;
  }

  // 发送消息
  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessage(message, type);
    }
  }

  // 发送消息到周围
  template <typename T>
  void SendMessageAround(const T &message, gateway::protocol::MessageType::type type) {
    if(this->actor_) {
      this->actor_->SendMessageAround(message, type);
    }
  }

  // 事件
  void OnEvent(event::EventType::type type, const void *data, size_t size);

  void DailyClean(core::int32 days);

  Task *GetTask(entity::TaskType::type type, core::uint32 id);
  
  // 玩家循环任务计算
  void CountCircleData(core::int32 days);
  
  // 玩家奇遇任务计算
  void CountTripData(core::int32 days);

  // 检查有没有符合相应NPC的采集任务
  bool CheckGatherTask(core::int32 template_id = 0);

  void AddLayer(core::uint32 task_id, bool sync = true);

  // 得到最大的纹章ID，如果失败, 返回 0
  core::int32 GetMaxLayerID() {
    if(this->layers_.size() == 0) {
      return 0;
    }
    return *(--this->layers_.end());
  }

  // 检查任务是否可以召唤NPC
  bool CheckSummonNpc(core::int32 npc_template);

 private:
  // 任务保存
  bool save_;

  // 玩家对象指针
  GameActor *actor_;

  // 当前正在进行的任务
  TaskMap tasks_;

  // 已完成的任务
  TaskHashset finish_tasks_;

  // 玩家循环任务对象
  TaskCircle circle_task_;

  // 玩家奇遇任务对象
  TaskTrip trip_task_;

  // 玩家每日任务
  TaskDaily task_daily_;

  // 纹章列表
  LayerMap layers_;

  // 护送
  TaskTransport task_transport_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__TASK__ACTOR__H

