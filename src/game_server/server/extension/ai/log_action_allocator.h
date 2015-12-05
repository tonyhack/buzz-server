//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-12 17:55:49.
// File name: log_action_allocator.h
//
// Description:
// Define class LogActionAllocator.
//

#ifndef __GAME__SERVER__AI__LOG__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__LOG__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/log_action.h"

namespace game {

namespace server {

namespace ai {

class LogActionAllocator : public ActionAllocator {
 public:
  LogActionAllocator();
  virtual ~LogActionAllocator();

  virtual LogAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__LOG__ACTION__ALLOCATOR__H

