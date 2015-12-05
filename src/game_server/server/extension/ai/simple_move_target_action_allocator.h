//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:41:58.
// File name: simple_move_target_action_allocator.h
//
// Description:
// Define class SimpleMoveTargetActionAllocator.
//

#ifndef __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/simple_move_target_action.h"

namespace game {

namespace server {

namespace ai {

class SimpleMoveTargetActionAllocator : public ActionAllocator {
 public:
  SimpleMoveTargetActionAllocator();
  virtual ~SimpleMoveTargetActionAllocator();

  virtual SimpleMoveTargetAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SIMPLE__MOVE__TARGET__ACTION__ALLOCATOR__H

