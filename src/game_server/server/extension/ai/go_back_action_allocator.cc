//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 13:43:06.
// File name: go_back_action_allocator.cc
//
// Description:
// Define class GoBackActionAllocator.
//

#include "game_server/server/extension/ai/go_back_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

GoBackActionAllocator::GoBackActionAllocator() {}
GoBackActionAllocator::~GoBackActionAllocator() {}

GoBackAction *GoBackActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  GoBackAction *action = new (std::nothrow) GoBackAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 GoBackAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 GoBackAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return action;
}

void GoBackActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

