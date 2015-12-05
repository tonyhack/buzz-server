//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:29:30.
// File name: check_interval_seconds_condition_allocator.h
//
// Description:
// Define class CheckIntervalSecondsConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_interval_seconds_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckIntervalSecondsConditionAllocator : public ConditionAllocator {
 public:
  CheckIntervalSecondsConditionAllocator();
  virtual ~CheckIntervalSecondsConditionAllocator();

  virtual CheckIntervalSecondsCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__INTERVAL__SECONDS__CONDITION__ALLOCATOR__H

