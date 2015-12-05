//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 19:16:01.
// File name: target_exist_condition_allocator.h
//
// Description:
// Define class TargetExistConditionAllocator.
//

#ifndef __GAME__SERVER__AI__TARGET__EXIST__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__TARGET__EXIST__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/target_exist_condition.h"

namespace game {

namespace server {

namespace ai {

class TargetExistConditionAllocator : public ConditionAllocator {
 public:
  TargetExistConditionAllocator();
  virtual ~TargetExistConditionAllocator();

  virtual TargetExistCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__TARGET__EXIST__CONDITION__ALLOCATOR__H

