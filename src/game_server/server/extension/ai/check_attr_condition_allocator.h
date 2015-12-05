//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:03:06.
// File name: check_attr_condition_allocator.h
//
// Description:
// Define class CheckAttrConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__ATTR__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__ATTR__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_attr_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckAttrConditionAllocator : public ConditionAllocator {
 public:
  CheckAttrConditionAllocator();
  virtual ~CheckAttrConditionAllocator();

  virtual CheckAttrCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__ATTR__CONDITION__ALLOCATOR__H

