//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 15:30:05.
// File name: talk_npc_condition.cc
//
// Description:
// Define class TalkNpcCondition.
//

#include "game_server/server/extension/task/talk_npc_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TalkNpcCondition::TalkNpcCondition() {}
TalkNpcCondition::~TalkNpcCondition() {}

bool TalkNpcCondition::Initialize(Task *task, core::uint32 param1, core::uint32 param2,
    core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 TalkNpcCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->npc_ = param1;
//  this->require_talk_ = param2;
//  this->talk_count_ = value;
  this->talk_ = value;
  return true;
}

void TalkNpcCondition::Finalize() {
  Condition::Finalize();
}

bool TalkNpcCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
//  var = this->npc_;
//  value = this-;
  var = this->npc_;
  value = this->talk_;
  return true;
}

bool TalkNpcCondition::CheckCondition() {
  return this->talk_ == 1 ? true : false;
}

void TalkNpcCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->npc_);
  var.__set_value_(this->talk_);

  this->task_->SynchronizeVariable(var);
}

void TalkNpcCondition::OnEvent(event::EventType::type type,
      const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ACTOR_TALK_NPC:
      this->OnEventTalkNpc(data, size);
      break;
    default:
      break;
  }
}

void TalkNpcCondition::OnEventTalkNpc(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventActorTalkNpc)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("[任务] 对话NPC事件");

  event::EventActorTalkNpc *event = (event::EventActorTalkNpc *)data;

  GameNpc *npc = GameNpcManager::GetInstance()->Get(event->npc_id_);
  if(npc == NULL) {
    return ;
  }

  if(npc->GetTemplateID() != this->npc_) {
    return ;
  }
  if(this->task_ == NULL) {
    return ;
  }
  if(this->task_->GetID() != (core::uint32)event->task_id_) {
    return ;
  }

  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

