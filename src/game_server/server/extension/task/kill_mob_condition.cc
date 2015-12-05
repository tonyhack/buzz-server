//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 11:47:12.
// File name: kill_mob_condition.cc
//
// Description:
// Define class KillMobCondition.
//

#include "game_server/server/extension/task/kill_mob_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

KillMobCondition::KillMobCondition() {}
KillMobCondition::~KillMobCondition() {}

bool KillMobCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 KillMobCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->npc_ = param1;
  this->require_kill_ = param2;
  this->killed_ = value;

  return true;
}

void KillMobCondition::Finalize() {
  Condition::Finalize();
}

bool KillMobCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = this->npc_;
  value = this->killed_;
  return true;
}

bool KillMobCondition::CheckCondition() {
  return this->killed_ >= this->require_kill_;
}

void KillMobCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_KILL_NPC:
      this->OnEventKillMob(data, size);
      break;
    default:
      break;
  }
}

void KillMobCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->npc_);
  var.__set_value_(this->killed_);

  this->task_->SynchronizeVariable(var);
}

void KillMobCondition::OnEventKillMob(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorKillNpc)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorKillNpc *event = (event::EventActorKillNpc *)data;

  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->npc_);
  if(npc == NULL) {
    return ;
  }

  if(npc->GetTemplateID() != this->npc_) {
    return ;
  }
  
  if(this->require_kill_ > this->killed_) {
    ++this->killed_;
    this->Synchronize();
  }

}

}  // namespace task

}  // namespace server

}  // namespace game

