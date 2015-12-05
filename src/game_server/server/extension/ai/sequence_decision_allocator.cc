//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-09 17:11:39.
// File name: sequence_decision_allocator.cc
//
// Description:
// Define class SequenceDecisionAllocator.
//

#include "game_server/server/extension/ai/sequence_decision_allocator.h"

#include "game_server/server/extension/ai/sequence_decision_node.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SequenceDecisionAllocator::SequenceDecisionAllocator() {}
SequenceDecisionAllocator::~SequenceDecisionAllocator() {}

BehaviourNode *SequenceDecisionAllocator::AllocateSelf(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  SequenceDecisionNode *decision = new (std::nothrow) SequenceDecisionNode();
  if(decision == NULL) {
    global::LogError("%s:%d (%s) 分配 SequenceDecisionNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(decision->Initialize(parent, node) == false) {
    delete decision;
    global::LogError("%s:%d (%s) 初始化 SequenceDecisionNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return decision;
}

void SequenceDecisionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

