//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:05:25.
// File name: check_attr_condition_allocator.cc
//
// Description:
// Define class CheckAttrConditionAllocator.
//

#include "game_server/server/extension/ai/check_attr_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckAttrConditionAllocator::CheckAttrConditionAllocator() {}
CheckAttrConditionAllocator::~CheckAttrConditionAllocator() {}

CheckAttrCondition *CheckAttrConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  CheckAttrCondition *condition = new (std::nothrow) CheckAttrCondition();
  if(condition == NULL) {
    LOG_ERROR("分配 CheckAttrCondition 失败");
    return NULL;
  }
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    LOG_ERROR("初始化 CheckAttrCondition 失败");
    return NULL;
  }
  return condition;
}

void CheckAttrConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

