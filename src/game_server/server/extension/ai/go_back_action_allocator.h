//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 13:40:34.
// File name: go_back_action_allocator.h
//
// Description:
// Define class GoBackActionAllocator.
//

#ifndef __GAME__SERVER__AI__GO__BACK__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__GO__BACK__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/go_back_action.h"

namespace game {

namespace server {

namespace ai {

class GoBackActionAllocator : public ActionAllocator {
 public:
  GoBackActionAllocator();
  virtual ~GoBackActionAllocator();

  virtual GoBackAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__GO__BACK__ACTION__ALLOCATOR__H

