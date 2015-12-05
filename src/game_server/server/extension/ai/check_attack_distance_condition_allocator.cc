//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 12:01:36.
// File name: check_attack_distance_condition_allocator.cc
//
// Description:
// Define class CheckAttackDistanceConditionAllocator.
//

#include "game_server/server/extension/ai/check_attack_distance_condition_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

CheckAttackDistanceConditionAllocator::CheckAttackDistanceConditionAllocator() {}
CheckAttackDistanceConditionAllocator::~CheckAttackDistanceConditionAllocator() {}

CheckAttackDistanceCondition *CheckAttackDistanceConditionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  CheckAttackDistanceCondition *condition = new (std::nothrow) CheckAttackDistanceCondition();
  if(condition == NULL) {
    LOG_ERROR("分配 CheckAttackDistanceCondition 失败");
    return NULL;
  }
  if(condition->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 CheckAttackDistanceCondition 失败");
    return NULL;
  }
  return condition;
}

void CheckAttackDistanceConditionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

