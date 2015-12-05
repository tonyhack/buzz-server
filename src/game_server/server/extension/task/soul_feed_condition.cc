//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 20:26:00.
// File name: soul_feed_condition.cc
//
// Description:
//

#include "game_server/server/extension/task/soul_feed_condition.h"

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

SoulFeedCondition::SoulFeedCondition() {}
SoulFeedCondition::~SoulFeedCondition() {}

bool SoulFeedCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 SoulFeedCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->required_num_ = param2;
  this->count_      = value;

  return true;
}

void SoulFeedCondition::Finalize() {
  Condition::Finalize();
}

bool SoulFeedCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = 0;
  value = this->count_;
  return true;
}

bool SoulFeedCondition::CheckCondition() {
  return this->count_ >= this->required_num_;
}

void SoulFeedCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  switch(type) {
    case event::EventType::EVENT_SOUL_UPGRADE_STEP:
      this->OnEventSoulUpgradeStep(data, size);
      break;
    default:
      break;
  }
}

void SoulFeedCondition::Synchronize() {
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

void SoulFeedCondition::OnEventSoulUpgradeStep(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventSoulUpgradeStep)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("[任务] 英灵培养事件");

  if(this->required_num_ > this->count_) {
    ++this->count_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

