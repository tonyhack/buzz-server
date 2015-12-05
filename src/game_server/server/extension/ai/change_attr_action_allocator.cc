//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 14:07:08.
// File name: change_attr_action_allocator.cc
//
// Description:
// Define class ChangeAttrActionAllocator.
//

#include "game_server/server/extension/ai/change_attr_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ChangeAttrActionAllocator::ChangeAttrActionAllocator() {}
ChangeAttrActionAllocator::~ChangeAttrActionAllocator() {}

ChangeAttrAction *ChangeAttrActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  ChangeAttrAction *action = new (std::nothrow) ChangeAttrAction();
  if(action == NULL) {
    LOG_ERROR("分配 ChangeAttrAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 ChangeAttrAction 失败");
    return NULL;
  }
  return action;
}

void ChangeAttrActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

