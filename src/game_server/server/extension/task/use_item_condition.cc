//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 23:36:47.
// File name: use_item_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/use_item_condition.h"

#include "game_server/server/extension/task/task.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_item_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

UseItemCondition::UseItemCondition() {}
UseItemCondition::~UseItemCondition() {}

bool UseItemCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 UseItemCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
   
  this->template_id_ = param1;
  this->require_num_ = param2;
  this->count_ = value;

  return true;
}

void UseItemCondition::Finalize() {
  Condition::Finalize();
}

bool UseItemCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = this->template_id_;
  value = this->count_;
  return true;
}

bool UseItemCondition::CheckCondition() {
  return this->count_ >= (core::uint32)this->require_num_; 
}

void UseItemCondition::Synchronize() {
  if(this->task_ == NULL) {
    global::LogError("%s:%d (%s) 同步任务数据时 task_ 为空",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 同步任务变量
  gateway::protocol::TaskVar var;
  var.__set_type_(this->GetType());
  var.__set_id_(this->template_id_);
  var.__set_value_(this->count_);

  this->task_->SynchronizeVariable(var);
}

void UseItemCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(type) {
    case event::EventType::EVENT_ITEM_USE:
      this->OnEventUseItem(data, size);
      break;
    default:
      break;
  }
}

void UseItemCondition::OnEventUseItem(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventItemUse)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventItemUse *use = (event::EventItemUse *)data;

  if(use->template_id_ != this->template_id_) {
    return ;
  }
  if(this->require_num_ > (core::int32)this->count_) {
    ++this->count_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

