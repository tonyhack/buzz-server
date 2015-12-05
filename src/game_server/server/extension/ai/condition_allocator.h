//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 19:15:00.
// File name: condition_allocator.h
//
// Description:
// Define class ConditionAllocator.
//

#ifndef __GAME__SERVER__AI__CONDITION__ALLOCATOR__H
#define __GAME__SERVER__AI__CONDITION__ALLOCATOR__H

#include "game_server/server/extension/ai/condition_node.h"
#include "game_server/server/extension/ai/allocator_template.h"

namespace game {

namespace server {

namespace ai {

class ConditionAllocator : public AllocatorTemplate<ConditionNode> {
 public:
  ConditionAllocator() {}
  virtual ~ConditionAllocator() {}
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CONDITION__ALLOCATOR__H

