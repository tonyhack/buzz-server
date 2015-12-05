//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 11:59:09.
// File name: check_attack_distance_condition_allocator.h
//
// Description:
// Define class CheckAttackDistanceConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__ATTACK__DISTANCE__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__ATTACK__DISTANCE__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_attack_distance_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckAttackDistanceConditionAllocator : public ConditionAllocator {
 public:
  CheckAttackDistanceConditionAllocator();
  virtual ~CheckAttackDistanceConditionAllocator();

  virtual CheckAttackDistanceCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__ATTACK__DISTANCE__CONDITION__ALLOCATOR__H

