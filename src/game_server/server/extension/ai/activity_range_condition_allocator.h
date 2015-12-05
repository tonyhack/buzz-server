//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 17:30:43.
// File name: activity_range_condition_allocator.h
//
// Description:
// Define class ActivityRangeConditionAllocator.
//

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/activity_range_condition.h"

namespace game {

namespace server {

namespace ai {

class ActivityRangeConditionAllocator : public ConditionAllocator {
 public:
  ActivityRangeConditionAllocator();
  virtual ~ActivityRangeConditionAllocator();

  virtual ActivityRangeCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

