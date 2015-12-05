//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 10:52:03.
// File name: selector_decision_allocator.cc
//
// Description:
// Define class SelectorDecisionAllocator.
//

#include "game_server/server/extension/ai/selector_decision_allocator.h"

#include "game_server/server/extension/ai/selector_decision_node.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SelectorDecisionAllocator::SelectorDecisionAllocator() : id_(0) {}
SelectorDecisionAllocator::~SelectorDecisionAllocator() {}

BehaviourNode *SelectorDecisionAllocator::AllocateSelf(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  SelectorDecisionNode *decision = new (std::nothrow) SelectorDecisionNode();
  if(decision == NULL) {
    global::LogError("%s:%d (%s) 分配 SelectorDecisionNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(decision->Initialize(parent, node) == false) {
    delete decision;
    global::LogError("%s:%d (%s) 初始化 SelectorDecisionNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  decision->SetID(++this->id_);

  return decision;
}

void SelectorDecisionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

