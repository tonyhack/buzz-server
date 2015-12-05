//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:24:17.
// File name: check_var_condition_allocator.h
//
// Description:
// Define class CheckVarConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__VAR__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__VAR__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_var_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckVarConditionAllocator : public ConditionAllocator {
 public:
  CheckVarConditionAllocator();
  virtual ~CheckVarConditionAllocator();

  virtual CheckVarCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__VAR__CONDITION__ALLOCATOR__H

