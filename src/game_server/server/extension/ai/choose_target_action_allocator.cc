//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-17 18:06:12.
// File name: choose_target_action_allocator.cc
//
// Description:
// Define class ChooseTargetActionAllocator.
//

#include "game_server/server/extension/ai/choose_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ChooseTargetActionAllocator::ChooseTargetActionAllocator() {}
ChooseTargetActionAllocator::~ChooseTargetActionAllocator() {}

ChooseTargetAction *ChooseTargetActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  ChooseTargetAction *action = new (std::nothrow) ChooseTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 ChooseTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 ChooseTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;
}

void ChooseTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace serve

}  // namespace game

