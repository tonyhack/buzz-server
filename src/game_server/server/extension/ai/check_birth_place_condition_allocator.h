//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 19:22:34.
// File name: check_birth_place_condition_allocator.h
//
// Description:
// Define class CheckBirthPlaceConditionAllocator.
//

#include "game_server/server/extension/ai/check_birth_place_condition.h"
#include "game_server/server/extension/ai/condition_allocator.h"

#ifndef __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__ALLOCATOR__H

namespace game {

namespace server {

namespace ai {

class CheckBirthPlaceConditionAllocator : public ConditionAllocator {
 public:
  CheckBirthPlaceConditionAllocator();
  virtual ~CheckBirthPlaceConditionAllocator();

  virtual CheckBirthPlaceCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__BIRTH__PLACE__CONDITION__ALLOCATOR__H

