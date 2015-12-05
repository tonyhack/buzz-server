//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 16:54:56.
// File name: sequence_decision_allocator.h
//
// Description:
// Define class SequenceDecisionAllocator.
//

#ifndef __GAME__SERVER__AI__SEQUENCE__DECISION__ALLOCATOR__H
#define __GAME__SERVER__AI__SEQUENCE__DECISION__ALLOCATOR__H

#include "game_server/server/extension/ai/decision_allocator.h"
#include "game_server/server/extension/ai/sequence_decision_node.h"

namespace game {

namespace server {

namespace ai {

class SequenceDecisionAllocator : public DecisionAllocator {
 public:
  SequenceDecisionAllocator();
  virtual ~SequenceDecisionAllocator();

  virtual void Deallocate(BehaviourNode *node);
 private:
  virtual BehaviourNode *AllocateSelf(BehaviourNode *parent, TiXmlElement *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SEQUENCE__DECISION__ALLOCATOR__H

