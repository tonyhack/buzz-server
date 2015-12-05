//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:37:08.
// File name: set_var_action_allocator.h
//
// Description:
// Define class SetVarActionAllocator.
//

#ifndef __GAME__SERVER__AI__SET__VAR__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SET__VAR__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/set_var_action.h"

namespace game {

namespace server {

namespace ai {

class SetVarActionAllocator : public ActionAllocator {
 public: 
  SetVarActionAllocator();
  virtual ~SetVarActionAllocator();

  virtual SetVarAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SET__VAR__ACTION__ALLOCATOR__H

