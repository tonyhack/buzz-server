//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 17:42:12.
// File name: soul_appear_condition.cc
//
// Description:
// Define class SoulAppearCondition.
//

#include "game_server/server/extension/task/soul_appear_condition.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

SoulAppearCondition::SoulAppearCondition() {}
SoulAppearCondition::~SoulAppearCondition() {}

bool SoulAppearCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulAppearCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->require_count_ = param2;
  this->count_ = value;

  return true;
}

void SoulAppearCondition::Finalize() {
  Condition::Finalize();
}

bool SoulAppearCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = 0;
  value = this->count_;
  return true;
}

bool SoulAppearCondition::CheckCondition() {
  return this->count_ >= this->require_count_;
}

void SoulAppearCondition::Synchronize() {
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

void SoulAppearCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_SKILL_USE_APPEAR:
      this->OnEventSoulAppear(data, size);
      break;
    default:
      break;
  }
}

void SoulAppearCondition::OnEventSoulAppear(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventUseAppearSkill)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("[任务] 幻化成功事件");
  if(this->require_count_ > this->count_) {
    ++this->count_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

