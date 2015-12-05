//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 14:41:25.
// File name: delete_me_action_allocator.cc
//
// Description:
// Define class DeleteMeActionAllocator.
//

#include "game_server/server/extension/ai/delete_me_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

DeleteMeActionAllocator::DeleteMeActionAllocator() {}
DeleteMeActionAllocator::~DeleteMeActionAllocator() {}

DeleteMeAction *DeleteMeActionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  DeleteMeAction *action = new (std::nothrow) DeleteMeAction();
  if(action == NULL) {
    LOG_ERROR("分配 DeleteMeAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 DeleteMeAction 失败");
    return NULL;
  }
  return action;
}

void DeleteMeActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

