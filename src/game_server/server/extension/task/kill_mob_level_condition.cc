
#include "game_server/server/extension/task/kill_mob_level_condition.h"

#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/types.h"

namespace game {

namespace server {

namespace task {

KillMobLevelCondition::KillMobLevelCondition() {}
KillMobLevelCondition::~KillMobLevelCondition() {}

bool KillMobLevelCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 KillMobLevelCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->level_ = param1;
  this->quired_num_ = param2;
  this->count_ = value;

  return true;
}

void KillMobLevelCondition::Finalize() {
  Condition::Finalize();
}

bool KillMobLevelCondition::ExportConditionVar(
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

  var = this->level_;
  value = this->count_;

  return true;
}

bool KillMobLevelCondition::CheckCondition() {
  if(this->task_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }

  return this->count_ >= this->quired_num_;
}

void KillMobLevelCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->level_);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);
}

void KillMobLevelCondition::OnEvent(event::EventType::type type,
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

void KillMobLevelCondition::OnEvent(const void *data, size_t size) {
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

  // 计算
  if(this->quired_num_ <= this->count_) {
    return ;
  }
  if(abs(event->npc_level_ - game_actor->GetAttribute(entity::RoleAoiFields::LEVEL)) <= level_) {
    this->count_++;
    // 同步任务数据
    this->Synchronize();
  }

}

void KillMobLevelCondition::Clear() {
}

}  // namespace task

}  // namespace server

}  // namespace game

