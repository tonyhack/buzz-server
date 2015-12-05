//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 09:53:08.
// File name: add_buff_action_allocator.cc
//
// Description:
// Define class AddBuffActionAllocator.
//

#include "game_server/server/extension/ai/add_buff_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

AddBuffActionAllocator::AddBuffActionAllocator() {}
AddBuffActionAllocator::~AddBuffActionAllocator() {}

AddBuffAction *AddBuffActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  AddBuffAction *action = new (std::nothrow) AddBuffAction();
  if(action == NULL) {
    LOG_ERROR("分配 AddBuffAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 AddBuffAction 失败");
    return NULL;
  }
  return action;
}

void AddBuffActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

