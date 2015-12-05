//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 14:04:28.
// File name: change_attr_action_allocator.h
//
// Description:
// Define class ChangeAttrActionAllocator.
//

#ifndef __GAME__SERVER__AI__CHANGE__ATTR__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__CHANGE__ATTR__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/change_attr_action.h"

namespace game {

namespace server {

namespace ai {

class ChangeAttrActionAllocator : public ActionAllocator {
 public:
  ChangeAttrActionAllocator();
  virtual ~ChangeAttrActionAllocator();

  virtual ChangeAttrAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__CHANGE__ATTR__ACTION__ALLOCATOR__H

