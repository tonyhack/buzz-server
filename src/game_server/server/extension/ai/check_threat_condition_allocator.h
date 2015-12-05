//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 17:48:14.
// File name: check_threat_condition_allocator.h
//
// Description:
// Define class CheckThreatConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CHECK__THREAT__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHECK__THREAT__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_allocator.h"
#include "game_server/server/extension/ai/check_threat_condition.h"

namespace game {

namespace server {

namespace ai {

class CheckThreatConditionAllocator : public ConditionAllocator {
 public:
  CheckThreatConditionAllocator();
  virtual ~CheckThreatConditionAllocator();

  virtual CheckThreatCondition *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHECK__THREAT__CONDITION__ALLOCATOR__H

