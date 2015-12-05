//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 14:15:35.
// File name: delete_me_action_allocator.h
//
// Description:
// Define class DeleteMeActionAllocator.
//

#ifndef __GAME__SERVER__AI__DELETE__ME__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__DELETE__ME__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/delete_me_action.h"

namespace game {

namespace server {

namespace ai {

class DeleteMeActionAllocator : public ActionAllocator {
 public:
  DeleteMeActionAllocator();
  virtual ~DeleteMeActionAllocator();

  virtual DeleteMeAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__DELETE__ME__ACTION__ALLOCATOR__H

