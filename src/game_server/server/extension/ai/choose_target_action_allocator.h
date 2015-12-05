//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 18:03:27.
// File name: choose_target_action_allocator.h
//
// Description:
// Define class ChooseTargetActionAllocator.
//

#ifndef __GAME__SERVER__AI__CHOOSE__TARGET__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHOOSE__TARGET__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/choose_target_action.h"

namespace game {

namespace server {

namespace ai {

class ChooseTargetActionAllocator : public ActionAllocator {
 public:
  ChooseTargetActionAllocator();
  virtual ~ChooseTargetActionAllocator();

  virtual ChooseTargetAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace serve

}  // namespace game

#endif  // __GAME__SERVER__AI__CHOOSE__TARGET__ACTION__ALLOCATOR__H

