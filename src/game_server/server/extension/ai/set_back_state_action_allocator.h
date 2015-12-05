//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 18:23:57.
// File name: set_back_state_action_allocator.h
//
// Description:
// Define class SetBackStateActionAllocator.
//

#ifndef __GAME__SERVER__AI__SET__BACK__STATE__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SET__BACK__STATE__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/set_back_state_action.h"

namespace game {

namespace server {

namespace ai {

class SetBackStateActionAllocator : public ActionAllocator {
 public:
  SetBackStateActionAllocator();
  virtual ~SetBackStateActionAllocator();

  virtual SetBackStateAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SET__BACK__STATE__ACTION__ALLOCATOR__H

