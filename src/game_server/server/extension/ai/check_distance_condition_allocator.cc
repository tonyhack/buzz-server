//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-16 17:25:58.
// File name: check_distance_condition_allocator.cc
//
// Description:
// Define class CheckDistanceConditionAllocator.
//

#include "game_server/server/extension/ai/check_distance_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckDistanceConditionAllocator::CheckDistanceConditionAllocator() {}
CheckDistanceConditionAllocator::~CheckDistanceConditionAllocator() {}

CheckDistanceCondition *CheckDistanceConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  CheckDistanceCondition *condition = new (std::nothrow) CheckDistanceCondition();
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 分配 CheckDistanceCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    global::LogError("%s:%d (%s) 初始化 CheckDistanceCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return condition;
}

void CheckDistanceConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

