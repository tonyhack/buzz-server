//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-11 00:46:59.
// File name: divine_gem_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/divine_gem_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

DivineGemCondition::DivineGemCondition() {}
DivineGemCondition::~DivineGemCondition() {}

bool DivineGemCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 DivineGemCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->quired_num_ = param2;
  this->count_      = value;

  return true;
}

void DivineGemCondition::Finalize() {
  Condition::Finalize();
}

bool DivineGemCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = 0;
  value = this->count_;
  return true;
}

bool DivineGemCondition::CheckCondition() {
  return this->count_ >= this->quired_num_;
}

void DivineGemCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ITEM_DIVINE_GEM:
      this->OnEventDivineGem(data, size);
      break;
    default:
      break;
  }
}

void DivineGemCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(0);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);
}

void DivineGemCondition::OnEventDivineGem(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventItemDivineGem)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("[任务] 宝石聚集");

  if(this->quired_num_ > this->count_) {
    ++this->count_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

