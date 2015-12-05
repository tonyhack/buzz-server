//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-12 00:15:57.
// File name: spirit_feed_condition.cc
//
// Description:
//
#include "game_server/server/extension/task/spirit_feed_condition.h"

#include "game_server/server/extension/task/task.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/event/game_attr_event_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

SpiritFeedCondition::SpiritFeedCondition() {}
SpiritFeedCondition::~SpiritFeedCondition() {}

bool SpiritFeedCondition::Initialize(Task *task, core::uint32 param1,
    core::uint32 param2, core::uint32 value) {
  if(Condition::Initialize(task, param1, param2, value) == false) {
    global::LogError("%s:%d (%s) 初始化 SpiritFeedCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->require_num_ = param2;
  this->count_ = value;

  return true;
}

void SpiritFeedCondition::Finalize() {
  Condition::Finalize();
}

bool SpiritFeedCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  var = this->require_num_;
  value = this->count_;
  return true;
}

bool SpiritFeedCondition::CheckCondition() {
  
  return this->count_ >= (core::uint32)this->require_num_; 
}

void SpiritFeedCondition::Synchronize() {
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

void SpiritFeedCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  if(data == NULL) {
    global::LogError("%s:%d (%s) 参数 data 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  switch(type) {
    case event::EventType::EVENT_ATTR_SPIRIT_FEED:
      this->OnEventFeed(data, size);
      break;
    default:
      break;
  }
}

void SpiritFeedCondition::OnEventFeed(const void *data, size_t size) {
  if(data == NULL || size != sizeof(event::EventAttrSpiritFeed)) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventAttrSpiritFeed *event = (event::EventAttrSpiritFeed *)data;
  if(this->require_num_ > (core::int32)this->count_) {
    ++this->count_;
  }
  // 同步任务数据
  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

