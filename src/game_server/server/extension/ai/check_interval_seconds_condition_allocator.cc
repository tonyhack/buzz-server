//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:32:41.
// File name: check_interval_seconds_condition_allocator.cc
//
// Description:
// Define class CheckIntervalSecondsConditionAllocator.
//

#include "game_server/server/extension/ai/check_interval_seconds_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckIntervalSecondsConditionAllocator::CheckIntervalSecondsConditionAllocator() {}
CheckIntervalSecondsConditionAllocator::~CheckIntervalSecondsConditionAllocator() {}

CheckIntervalSecondsCondition *CheckIntervalSecondsConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  CheckIntervalSecondsCondition *condition = new (std::nothrow) CheckIntervalSecondsCondition();
  if(condition == NULL) {
    LOG_ERROR("分配 CheckIntervalSecondsCondition 失败");
    return NULL;
  }
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    LOG_ERROR("分配 CheckIntervalSecondsCondition 失败");
    return NULL;
  }
  return condition;
}

void CheckIntervalSecondsConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

