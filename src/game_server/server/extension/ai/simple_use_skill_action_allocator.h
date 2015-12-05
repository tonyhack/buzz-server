//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-09 12:24:01.
// File name: simple_use_skill_action_allocator.h
//
// Description:
// Define class SimpleUseSkillActionAllocator.
//

#ifndef __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/simple_use_skill_action.h"

namespace game {

namespace server {

namespace ai {

class SimpleUseSkillActionAllocator : public ActionAllocator {
 public:
  SimpleUseSkillActionAllocator();
  virtual ~SimpleUseSkillActionAllocator();

  virtual SimpleUseSkillAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SIMPLE__USE__SKILL__ACTION__ALLOCATOR__H

