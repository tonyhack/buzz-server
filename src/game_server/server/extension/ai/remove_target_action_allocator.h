//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:39:55.
// File name: remove_target_action_allocator.h
//
// Description:
// Define class RemoveTargetActionAllocator.
//

#ifndef __GAME__SERVER__AI__REMOVE__TARGET__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__REMOVE__TARGET__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/remove_target_action.h"

namespace game {

namespace server {

namespace ai {

class RemoveTargetActionAllocator : public ActionAllocator {
 public:
  RemoveTargetActionAllocator();
  virtual ~RemoveTargetActionAllocator();

  virtual RemoveTargetAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__REMOVE__TARGET__ACTION__ALLOCATOR__H

