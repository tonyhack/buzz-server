//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 09:58:16.
// File name: decision_allocator.h
//
// Description:
// Define class DecisionAllocator.
//

#ifndef __GAME__SERVER__AI__DECISION__ALLOCATOR__H
#define __GAME__SERVER__AI__DECISION__ALLOCATOR__H

#include "game_server/server/extension/ai/allocator_template.h"

namespace game {

namespace server {

namespace ai {

class BehaviourNode;

class DecisionAllocator : public AllocatorTemplate<BehaviourNode> {
 public:
  DecisionAllocator();
  virtual ~DecisionAllocator();

  virtual BehaviourNode *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node) = 0;

 protected:
  virtual BehaviourNode *AllocateSelf(BehaviourNode *parent, TiXmlElement *node) = 0;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__DECISION__ALLOCATOR__H

