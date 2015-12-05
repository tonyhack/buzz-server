//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 18:26:27.
// File name: set_back_state_action_allocator.cc
//
// Description:
// Define class SetBackStateActionAllocator.
//

#include "game_server/server/extension/ai/set_back_state_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SetBackStateActionAllocator::SetBackStateActionAllocator() {}
SetBackStateActionAllocator::~SetBackStateActionAllocator() {}

SetBackStateAction *SetBackStateActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  SetBackStateAction *action = new (std::nothrow) SetBackStateAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 SetBackStateAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 SetBackStateAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return action;
}

void SetBackStateActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

