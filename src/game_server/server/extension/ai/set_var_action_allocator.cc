//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-06 15:39:30.
// File name: set_var_action_allocator.cc
//
// Description:
// Define class SetVarActionAllocator.
//

#include "game_server/server/extension/ai/set_var_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SetVarActionAllocator::SetVarActionAllocator() {}
SetVarActionAllocator::~SetVarActionAllocator() {}

SetVarAction *SetVarActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  SetVarAction *action = new (std::nothrow) SetVarAction();
  if(action == NULL) {
    LOG_ERROR("分配 SetVarAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 SetVarAction 失败");
    return NULL;
  }
  return action;
}

void SetVarActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

