//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 16:11:09.
// File name: summon_npc_action_allocator.cc
//
// Description:
// Define class SummonNpcActionAllocator.
//

#include "game_server/server/extension/ai/summon_npc_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SummonNpcActionAllocator::SummonNpcActionAllocator() {}
SummonNpcActionAllocator::~SummonNpcActionAllocator() {}

SummonNpcAction *SummonNpcActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  SummonNpcAction *action = new (std::nothrow) SummonNpcAction();
  if(action == NULL) {
    LOG_ERROR("分配 SummonNpcAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 SummonNpcAction 失败");
    return NULL;
  }
  return action;
}

void SummonNpcActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game
