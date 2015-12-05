//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 18:03:17.
// File name: check_back_state_condition_allocator.h
//
// Description:
// Define class CheckBackStateConditionAllocator.
//

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_back_state_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckBackStateConditionAllocator : public ConditionAllocator {
 public:
  CheckBackStateConditionAllocator();
  virtual ~CheckBackStateConditionAllocator();

  virtual CheckBackStateCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

