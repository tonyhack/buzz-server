//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 16:53:11.
// File name: behaviour_node_factory.cc
//
// Description:
// Define class BehaviourNodeFactory.
//

#include "game_server/server/extension/ai/behaviour_node_factory.h"

#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/decision_allocator.h"

#include <string.h>

namespace game {

namespace server {

namespace ai {

const char *BehaviourNodeFactory::kDecisionTypeStr_ = "decision";
const size_t BehaviourNodeFactory::kStrlenDecision_ = 9;

const char *BehaviourNodeFactory::kBehaviourTypeStr_ = "behaviour";
const size_t BehaviourNodeFactory::kStrlenBehaviour_ = 10;

const char *BehaviourNodeFactory::kConditionTypeStr_ = "condition";
const size_t BehaviourNodeFactory::kStrlenCondition_ = 10;

const char *BehaviourNodeFactory::kActionTypeStr_ = "action";
const size_t BehaviourNodeFactory::kStrlenAction_ = 7;

BehaviourNode *BehaviourNodeFactory::Allocate(const char *type1, const char *type2,
    const char *name, BehaviourNode *parent_node, TiXmlElement *node) {
  if(strncmp(type1, kDecisionTypeStr_, kStrlenDecision_) == 0) {
    // 策略结点
    DecisionAllocator *allocator = DecisionFactory::GetInstance()->GetAllocator(type2);
    if(allocator) {
      return allocator->Allocate(parent_node, node);
    }
  } else if(strncmp(type1, kBehaviourTypeStr_, kStrlenBehaviour_) == 0) {
    // 行为结点
    if(strncmp(type2, kConditionTypeStr_, kStrlenCondition_) == 0) {
      // 条件结点
      ConditionAllocator *allocator = ConditionFactory::GetInstance()->GetAllocator(name);
      if(allocator) {
        return allocator->Allocate(parent_node, node);
      }
    } else if(strncmp(type2, kActionTypeStr_, kStrlenAction_) == 0) {
      // 动作结点
      ActionAllocator *allocator = ActionFactory::GetInstance()->GetAllocator(name);
      if(allocator) {
        return allocator->Allocate(parent_node, node);
      }
    }
  }
  return NULL;
}

}  // namespace ai

}  // namespace server

}  // namespace game

