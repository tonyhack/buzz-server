//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 22:25:56.
// File name: soul_equip_condition.cc
//
// Description:
//
#include "game_server/server/extension/task/soul_equip_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

SoulEquipCondition::SoulEquipCondition() {}
SoulEquipCondition::~SoulEquipCondition() {}

bool SoulEquipCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulEquipCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->required_count_ = param2;
  this->num_   = value;

  return true;
}

void SoulEquipCondition::Finalize() {
  Condition::Finalize();
}

bool SoulEquipCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = 0;
  value = this->num_;
  return true;
}

bool SoulEquipCondition::CheckCondition() {
  return this->num_ >= this->required_count_;
}

void SoulEquipCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(type) {
    case event::EventType::EVENT_SOUL_SET:
      this->OnEventSoulSet(data, size);
      break;
    default:
      break;
  }
}

void SoulEquipCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(0);
  var.__set_value_(this->num_);

  this->task_->SynchronizeVariable(var);
}

void SoulEquipCondition::OnEventSoulSet(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSoulSet)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(this->required_count_ > this->num_) {
    ++this->num_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

