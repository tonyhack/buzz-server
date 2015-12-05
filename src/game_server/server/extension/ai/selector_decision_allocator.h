//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 09:52:42.
// File name: selector_decision_allocator.h
//
// Description:
// Define class SelectorDecisionAllocator.
//

#ifndef __GAME__SERVER__AI__SELECTOR__DECISION__ALLOCATOR__H
#define __GAME__SERVER__AI__SELECTOR__DECISION__ALLOCATOR__H

#include "game_server/server/extension/ai/decision_allocator.h"
#include "game_server/server/extension/ai/selector_decision_node.h"

namespace game {

namespace server {

namespace ai {

class SelectorDecisionAllocator : public DecisionAllocator {
 public:
  SelectorDecisionAllocator();
  virtual ~SelectorDecisionAllocator();

  virtual void Deallocate(BehaviourNode *node);

 private:
  virtual BehaviourNode *AllocateSelf(BehaviourNode *parent, TiXmlElement *node);

  core::uint32 id_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SELECTOR__DECISION__ALLOCATOR__H

