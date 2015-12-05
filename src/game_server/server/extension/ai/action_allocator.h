//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:25:44.
// File name: action_allocator.h
//
// Description:
// Define class ActionAllocator.
//

#ifndef __GAME__SERVER__AI__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_node.h"
#include "game_server/server/extension/ai/allocator_template.h"

namespace game {

namespace server {

namespace ai {

class ActionAllocator : public AllocatorTemplate<ActionNode> {
 public:
  ActionAllocator() {}
  virtual ~ActionAllocator() {}
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ACTION__ALLOCATOR__H

