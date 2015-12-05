
#include "game_server/server/extension/task/get_resource_condition.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

GetResourceCondition::GetResourceCondition() {}
GetResourceCondition::~GetResourceCondition() {}

bool GetResourceCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 GetResourceCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->count_ = value;
  this->quired_num_ = param2;
  this->resource_id_ = param1;

  return true;
}

void GetResourceCondition::Finalize() {
  Condition::Finalize();
}

bool GetResourceCondition::ExportConditionVar(
    core::uint32 &var, core::uint32 &value) { 
  // 获取任务玩家对象
  if(this->task_ == NULL) {
    LOG_ERROR("玩家的任务指针没有找到");
    return false;
  }
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return false;
  }

  // 获取玩家对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家对象失败");
    return false;
  }

  var = this->resource_id_;
  value = this->count_;

  return true;
}

bool GetResourceCondition::CheckCondition() {
  if(this->task_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }

  // 获取任务玩家对象
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return false;
  }
  return this->count_ >= this->quired_num_;
}

void GetResourceCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->resource_id_);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);
}

void GetResourceCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_RESOURCE_CHANGE:
      this->OnEvent(data, size);
      break;
    default:
      break;
  }
}

void GetResourceCondition::OnEvent(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorResourceChange)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorResourceChange *event = (event::EventActorResourceChange *)data;

  // 获取任务玩家对象
  TaskActor *task_actor = this->task_->GetActor();
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象获取失败");
    return ;
  }

  // 获取玩家对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家对象失败");
    return ;
  }

  if((core::uint32)event->type_ != this->resource_id_) {
    return ;
  }

  if(event->old_num_ < event->new_num_ && this->quired_num_ > this->count_) {
    this->count_ = event->new_num_ - event->old_num_;
    this->Synchronize();
  }
}

void GetResourceCondition::Clear() {
}

}  // namespace task

}  // namespace server

}  // namespace game

