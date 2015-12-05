//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 16:09:10.
// File name: summon_npc_action_allocator.h
//
// Description:
// Define class SummonNpcActionAllocator.
//

#ifndef __GAME__SERVER__AI__SUMMON__NPC__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SUMMON__NPC__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/summon_npc_action.h"

namespace game {

namespace server {

namespace ai {

class SummonNpcActionAllocator : public ActionAllocator {
 public:
  SummonNpcActionAllocator();
  virtual ~SummonNpcActionAllocator();

  virtual SummonNpcAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SUMMON__NPC__ACTION__ALLOCATOR__H

