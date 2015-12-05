//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-05 17:42:31.
// File name: reset_interval_seconds_action_allocator.cc
//
// Description:
// Define class ResetIntervalSecondsActionAllocator.
//

#include "game_server/server/extension/ai/reset_interval_seconds_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ResetIntervalSecondsActionAllocator::ResetIntervalSecondsActionAllocator() {}
ResetIntervalSecondsActionAllocator::~ResetIntervalSecondsActionAllocator() {}

ResetIntervalSecondsAction *ResetIntervalSecondsActionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  ResetIntervalSecondsAction *action = new (std::nothrow) ResetIntervalSecondsAction();
  if(action == NULL) {
    LOG_ERROR("分配 ResetIntervalSecondsAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 ResetIntervalSecondsAction 失败");
    return NULL;
  }
  return action;
}

void ResetIntervalSecondsActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

