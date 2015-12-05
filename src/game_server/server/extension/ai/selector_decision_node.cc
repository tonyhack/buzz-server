//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 13:43:51.
// File name: selector_decision_node.cc
//
// Description:
// Define class SelectorDecisionNode.
//

#include "game_server/server/extension/ai/selector_decision_node.h"

#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/black_board.h"
#include "game_server/server/extension/ai/selector_decision_allocator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SelectorDecisionNode::SelectorDecisionNode()
  : current_child_index_(kChildrenNumber) {}

SelectorDecisionNode::~SelectorDecisionNode() {}

bool SelectorDecisionNode::Initialize(BehaviourNode *parent, TiXmlElement *node) {
  if(BehaviourNode::Initialize(parent, node) == false) {
    global::LogError("%s:%d (%s) 初始 BehaviourNode 部分失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->current_child_index_ = kChildrenNumber;
  // this->last_child_index_ = kChildrenNumber;
  return true;
}

void SelectorDecisionNode::Deallocate() {
  BehaviourNode::Deallocate();
  SelectorDecisionAllocator *allocator =
    (SelectorDecisionAllocator *)DecisionFactory::GetInstance()->GetAllocator("selector");
  if(allocator == NULL) {
    global::LogError("%s:%d (%s) 获取 SelectorDecisionAllocator 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  allocator->Deallocate(this);
}

bool SelectorDecisionNode::CheckEvaluate(BlackBoard *black_board) const {
  // 寻找第一个满足条件的结点
  this->current_child_index_ = 0;
  NodeVector::const_iterator iterator = this->children_.begin();
  for(; iterator != this->children_.end(); ++iterator, ++this->current_child_index_) {
    // 有一个条件满足就返回
    if((*iterator)->CheckEvaluate(black_board) == true) {
      return true;
    }
  }
  // 没有满足，把当前索引清空
  this->current_child_index_ = kChildrenNumber;
  return false;
}

void SelectorDecisionNode::OnTransition(BlackBoard *black_board) {
  // 转移结点需要把上次的执行结点清空
  size_t &last_child_index = black_board->GetSelectorLastChild(this->id_);
  if(this->CheckIndexValid(last_child_index)) {
    this->children_[last_child_index]->OnTransition(black_board);
  }
  last_child_index = kChildrenNumber;
}

RunningStatus::type SelectorDecisionNode::OnRunning(BlackBoard *black_board) {
  RunningStatus::type ret = RunningStatus::FINISH;
  size_t &last_child_index = black_board->GetSelectorLastChild(this->id_);
  // 当前索引合法时需要先把上次索引结点清空
  if(this->CheckIndexValid(this->current_child_index_)) {
    if(this->current_child_index_ != last_child_index) {
      if(this->CheckIndexValid(last_child_index)) {
        this->children_[last_child_index]->OnTransition(black_board);
      }
      last_child_index = this->current_child_index_;
    }
  }
  // 执行上次索引(如果满足条件的是新的结点，执行的会是新的结点)
  if(this->CheckIndexValid(last_child_index)) {
    ret = this->children_[last_child_index]->OnRunning(black_board);
    if(ret == RunningStatus::FINISH) {
      last_child_index = kChildrenNumber;
    }
  }
  return ret;
}

}  // namespace ai

}  // namespace server

}  // namespace game

