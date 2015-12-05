//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 21:09:06.
// File name: move_target_action_allocator.cc
//
// Description:
// Define class MoveTargetActionAllocator.
//

#include "game_server/server/extension/ai/move_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

MoveTargetActionAllocator::MoveTargetActionAllocator() {}
MoveTargetActionAllocator::~MoveTargetActionAllocator() {}

MoveTargetAction *MoveTargetActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  MoveTargetAction *action = new (std::nothrow) MoveTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 MoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 MoveTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;

}

void MoveTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

