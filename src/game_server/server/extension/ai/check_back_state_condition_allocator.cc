//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 19:24:50.
// File name: check_back_state_condition_allocator.cc
//
// Description:
// Define class CheckBackStateConditionAllocator.
//

#include "game_server/server/extension/ai/check_back_state_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckBackStateConditionAllocator::CheckBackStateConditionAllocator() {}
CheckBackStateConditionAllocator::~CheckBackStateConditionAllocator() {}

CheckBackStateCondition *CheckBackStateConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  CheckBackStateCondition *condition =
    new (std::nothrow) CheckBackStateCondition();
  if(condition == NULL) {
    global::LogError("%s:%d (%s) 分配 CheckBackStateCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(condition->Initialize(parent, node) == false) {
    delete condition;
    global::LogError("%s:%d (%s) 初始化 CheckBackStateCondition 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return condition;
}

void CheckBackStateConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

