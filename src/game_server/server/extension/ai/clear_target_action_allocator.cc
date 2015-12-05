//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-09 13:57:17.
// File name: clear_target_action_allocator.cc
//
// Description:
// Define class ClearTargetActionAllocator.
//

#include "game_server/server/extension/ai/clear_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ClearTargetActionAllocator::ClearTargetActionAllocator() {}
ClearTargetActionAllocator::~ClearTargetActionAllocator() {}

ClearTargetAction *ClearTargetActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  ClearTargetAction *action = new (std::nothrow) ClearTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 ClearTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 ClearTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return action;
}

void ClearTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

