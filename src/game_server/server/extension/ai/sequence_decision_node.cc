//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 11:13:41.
// File name: sequence_decision_node.cc
//
// Description:
// Define class SequenceDecisionNode.
//

#include "game_server/server/extension/ai/sequence_decision_node.h"

#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/sequence_decision_allocator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SequenceDecisionNode::SequenceDecisionNode() {}
SequenceDecisionNode::~SequenceDecisionNode() {}

bool SequenceDecisionNode::Initialize(BehaviourNode *parent, TiXmlElement *node) {
  if(BehaviourNode::Initialize(parent, node) == false) {
    global::LogError("%s:%d (%s) 初始 BehaviourNode 部分失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // 初始化时把所有子结点设置为正在运行
  NodeVector::iterator iterator = this->children_.begin();
  for(; iterator != this->children_.end(); ++iterator) {
    (*iterator)->SetRunningStatus(RunningStatus::EXECUTING);
  }
  return true;
}

void SequenceDecisionNode::Deallocate() {
  BehaviourNode::Deallocate();
  SequenceDecisionAllocator *allocator =
    (SequenceDecisionAllocator *)DecisionFactory::GetInstance()->GetAllocator("sequence");
  if(allocator == NULL) {
    global::LogError("%s:%d (%s) 获取 SequenceDecisionNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  allocator->Deallocate(this);
}

bool SequenceDecisionNode::CheckEvaluate(BlackBoard *black_board) const {
  NodeVector::const_iterator iterator = this->children_.begin();
  for(; iterator != this->children_.end(); ++iterator) {
    // 只有在运行中的结点才检测是否满足条件
    if((*iterator)->GetRunningStatus() == RunningStatus::EXECUTING) {
      if((*iterator)->CheckEvaluate(black_board) == false) {
        return false;
      }
    }
  }
  return true;
}

void SequenceDecisionNode::OnTransition(BlackBoard *black_board) {
  // 结点转移时，重置所有子结点为正在运行状态
  NodeVector::iterator iterator = this->children_.begin();
  for(; iterator != this->children_.end(); ++iterator) {
    (*iterator)->OnTransition(black_board);
    (*iterator)->SetRunningStatus(RunningStatus::EXECUTING);
  }
}

RunningStatus::type SequenceDecisionNode::OnRunning(BlackBoard *black_board) {
  size_t finish_number = 0;
  NodeVector::iterator iterator = this->children_.begin();
  for(; iterator != this->children_.end(); ++iterator) {
    // 只有在运行中的结点才需要执行
    if((*iterator)->GetRunningStatus() == RunningStatus::EXECUTING) {
      RunningStatus::type ret = (*iterator)->OnRunning(black_board);
      if(ret == RunningStatus::ERROR) {
        break;
      } else if(ret == RunningStatus::FINISH) {
        ++finish_number;
      }
    }
  }
  // 全部结点运行成功
  if(finish_number == this->children_.size()) {
    // 所有子结点状态设置为正在运行，保证下次进入时，逻辑重新执行
    NodeVector::iterator iterator = this->children_.begin();
    for(; iterator != this->children_.end(); ++iterator) {
      (*iterator)->SetRunningStatus(RunningStatus::EXECUTING);
    }
    return RunningStatus::FINISH;
  }
  return RunningStatus::EXECUTING;
}

void SequenceDecisionNode::OnAddChildNode(BehaviourNode *node) {
  // 新增加结点设置为正在运行
  if(node) {
    node->SetRunningStatus(RunningStatus::EXECUTING);
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

