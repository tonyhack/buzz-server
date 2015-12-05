//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 09:50:25.
// File name: add_buff_action_allocator.h
//
// Description:
// Define class AddBuffActionAllocator.
//

#ifndef __GAME__SERVER__AI__ADD__BUFF__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__ADD__BUFF__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/add_buff_action.h"

namespace game {

namespace server {

namespace ai {

class AddBuffActionAllocator : public ActionAllocator {
 public:
  AddBuffActionAllocator();
  virtual ~AddBuffActionAllocator();

  virtual AddBuffAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ADD__BUFF__ACTION__ALLOCATOR__H

