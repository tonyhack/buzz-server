//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-16 17:22:32.
// File name: check_distance_condition_allocator.h
//
// Description:
// Define class CheckDistanceConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__DISTANCE__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__DISTANCE__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_distance_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckDistanceConditionAllocator : public ConditionAllocator {
 public:
  CheckDistanceConditionAllocator();
  virtual ~CheckDistanceConditionAllocator();

  virtual CheckDistanceCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__DISTANCE__CONDITION__ALLOCATOR__H

