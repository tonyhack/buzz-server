//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 17:50:48.
// File name: check_threat_condition_allocator.cc
//
// Description:
// Define class CheckAttrConditionAllocator.
//

#include "game_server/server/extension/ai/check_threat_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckThreatConditionAllocator::CheckThreatConditionAllocator() {}
CheckThreatConditionAllocator::~CheckThreatConditionAllocator() {}

CheckThreatCondition *CheckThreatConditionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  CheckThreatCondition *condition = new (std::nothrow) CheckThreatCondition();
  if(condition == NULL) {
    LOG_ERROR("分配 CheckThreatCondition 失败");
    return NULL;
  }
  if(condition->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 CheckThreatCondition 失败");
    return NULL;
  }
  return condition;
}

void CheckThreatConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

