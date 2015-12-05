//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:43:52.
// File name: remove_target_action_allocator.cc
//
// Description:
// Define class RemoveTargetActionAllocator.
//

#include "game_server/server/extension/ai/remove_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RemoveTargetActionAllocator::RemoveTargetActionAllocator() {}
RemoveTargetActionAllocator::~RemoveTargetActionAllocator() {}

RemoveTargetAction *RemoveTargetActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  RemoveTargetAction *action = new (std::nothrow) RemoveTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 RemoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 RemoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;
}

void RemoveTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace sever

}  // namespace game

