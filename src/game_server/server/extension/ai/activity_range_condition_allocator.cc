//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 17:35:47.
// File name: activity_range_condition_allocator.cc
//
// Description:
// Define class ActivityRangeConditionAllocator.
//

#include "game_server/server/extension/ai/activity_range_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ActivityRangeConditionAllocator::ActivityRangeConditionAllocator() {}

ActivityRangeConditionAllocator::~ActivityRangeConditionAllocator() {}

ActivityRangeCondition *ActivityRangeConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  ActivityRangeCondition *condition = new (std::nothrow) ActivityRangeCondition();
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 分配 ActivityRangeCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    global::LogError("%s:%d (%s) 初始化 ActivityRangeCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return condition;
}

void ActivityRangeConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

