//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-17 16:47:50.
// File name: search_target_action_allocator.cc
//
// Description:
// Define class SearchTargetActionAllocator.
//

#include "game_server/server/extension/ai/search_target_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SearchTargetActionAllocator::SearchTargetActionAllocator() {}
SearchTargetActionAllocator::~SearchTargetActionAllocator() {}

SearchTargetAction *SearchTargetActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  SearchTargetAction *action = new (std::nothrow) SearchTargetAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 SearchTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 SearchTargetAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;
}

void SearchTargetActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

