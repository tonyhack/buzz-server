//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-24 00:28:50.
// File name: summon_npc_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/summon_npc_condition.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task_extension.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

SummonNpcCondition::SummonNpcCondition() {}
SummonNpcCondition::~SummonNpcCondition() {}

bool SummonNpcCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 SummonNpcCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->require_id_ = param1;
  this->count_ = value;
  this->num_ = param2;

  return true;
}

void SummonNpcCondition::Finalize() {
  Condition::Finalize();
}

bool SummonNpcCondition::ExportConditionVar(
    core::uint32 &var, core::uint32 &value) { 
  var = this->require_id_;
  value = this->count_;

  return true;
}

bool SummonNpcCondition::CheckCondition() {
  if(this->count_ < (core::uint32)this->num_) {
    return false;
  }

  return true;
}

void SummonNpcCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->require_id_);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);
}

void SummonNpcCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_KILL_NPC:
      this->OnEvent(data, size);
      break;
    default:
      break;
  }
}

void SummonNpcCondition::OnEvent(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorKillNpc)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorKillNpc *event = (event::EventActorKillNpc *)data;

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

  if(TaskExtension::GetInstance()->GetSummonNpcManager().GetNpcID(
      this->require_id_, game_actor->GetScene()) != (core::uint64)event->npc_) {
    return ;
  }
  ++this->count_;

  // 同步任务数据
  this->Synchronize();
}

void SummonNpcCondition::Clear() {
}


}  // namespace task

}  // namespace server

}  // namespace game

