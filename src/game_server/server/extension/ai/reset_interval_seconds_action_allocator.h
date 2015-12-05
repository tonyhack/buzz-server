//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:39:32.
// File name: reset_interval_seconds_action_allocator.h
//
// Description:
// Define class ResetIntervalSecondsActionAllocator.
//

#ifndef __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/reset_interval_seconds_action.h"

namespace game {

namespace server {

namespace ai {

class ResetIntervalSecondsActionAllocator : public ActionAllocator {
 public:
  ResetIntervalSecondsActionAllocator();
  virtual ~ResetIntervalSecondsActionAllocator();

  virtual ResetIntervalSecondsAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__RESET__INTERVAL__SECONDS__ACTION__ALLOCATOR__H

