//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:45:38.
// File name: search_target_action_allocator.h
//
// Description:
// Define class SearchTargetActionAllocator.
//

#ifndef __GAME__SERVER__AI__SEARCH__TARGET__ACTION__ALLOCATOR__H
#define __GAME__SERVER__AI__SEARCH__TARGET__ACTION__ALLOCATOR__H

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/search_target_action.h"

namespace game {

namespace server {

namespace ai {

class SearchTargetActionAllocator : public ActionAllocator {
 public:
  SearchTargetActionAllocator();
  virtual ~SearchTargetActionAllocator();

  virtual SearchTargetAction *Allocate(BehaviourNode *parent, TiXmlElement *node);
  virtual void Deallocate(BehaviourNode *node);
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__SEARCH__TARGET__ACTION__ALLOCATOR__H

