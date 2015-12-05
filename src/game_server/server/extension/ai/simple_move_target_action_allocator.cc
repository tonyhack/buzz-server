//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:42:39.
// File name: simple_move_target_action_allocator.cc
//
// Description:
// Define class SimpleMoveTargetActionAllocator.
//

#include "game_server/server/extension/ai/simple_move_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SimpleMoveTargetActionAllocator::SimpleMoveTargetActionAllocator() {}

SimpleMoveTargetActionAllocator::~SimpleMoveTargetActionAllocator() {}

SimpleMoveTargetAction *SimpleMoveTargetActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  SimpleMoveTargetAction *action = new (std::nothrow) SimpleMoveTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 SimpleMoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 SimpleMoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;
}

void SimpleMoveTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

