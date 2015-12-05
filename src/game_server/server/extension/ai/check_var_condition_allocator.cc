//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:26:46.
// File name: check_var_condition_allocator.cc
//
// Description:
// Define class CheckVarConditionAllocator.
//

#include "game_server/server/extension/ai/check_var_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckVarConditionAllocator::CheckVarConditionAllocator() {}
CheckVarConditionAllocator::~CheckVarConditionAllocator() {}

CheckVarCondition *CheckVarConditionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  CheckVarCondition *condition = new (std::nothrow) CheckVarCondition();
  if(condition == NULL) {
    LOG_ERROR("分配 CheckVarCondition 失败");
    return NULL;
  }
  if(condition->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 CheckVarCondition 失败");
    return NULL;
  }
  return condition;
}

void CheckVarConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

