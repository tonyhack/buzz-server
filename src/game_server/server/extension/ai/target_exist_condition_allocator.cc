//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 19:26:54.
// File name: target_exist_condition_allocator.cc
//
// Description:
// Define class TargetExistConditionAllocator.
//

#include "game_server/server/extension/ai/target_exist_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

TargetExistConditionAllocator::TargetExistConditionAllocator() {}
TargetExistConditionAllocator::~TargetExistConditionAllocator() {}

TargetExistCondition *TargetExistConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  TargetExistCondition *condition = new (std::nothrow) TargetExistCondition();
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 分配 TargetExistCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    global::LogError("%s:%d (%s) 初始化 TargetExistCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return condition;
}

void TargetExistConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

