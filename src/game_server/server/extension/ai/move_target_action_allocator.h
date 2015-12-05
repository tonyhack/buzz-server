//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 21:08:01.
// File name: move_target_action_allocator.h
//
// Description:
// Define class MoveTargetActionAllocator.
//

#ifndef __GAME__SERVER__AI__MOVE__TARGET__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__MOVE__TARGET__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/move_target_action.h"

namespace game {

namespace server {

namespace ai {

class MoveTargetActionAllocator : public ActionAllocator {
 public:
  MoveTargetActionAllocator();
  virtual ~MoveTargetActionAllocator();

  virtual MoveTargetAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__MOVE__TARGET__ACTION__ALLOCATOR__H

