//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 13:55:07.
// File name: random_move_action_allocator.h
//
// Description:
// Define class RandomMoveActionAllocator.
//

#ifndef __GAME__SERVER__AI__RANDOM__MOVE__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__RANDOM__MOVE__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/random_move_action.h"

namespace game {

namespace server {

namespace ai {

class RandomMoveActionAllocator : public ActionAllocator {
 public:
  RandomMoveActionAllocator();
  virtual ~RandomMoveActionAllocator();

  virtual RandomMoveAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__RANDOM__MOVE__ACTION__ALLOCATOR__H

