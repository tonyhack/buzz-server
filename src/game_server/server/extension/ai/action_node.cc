//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 14:45:28.
// File name: action_node.cc
//
// Description:
// Define class ActionNode.
//

#include "game_server/server/extension/ai/action_node.h"

#include "game_server/server/extension/ai/action_allocator.h"
#include "game_server/server/extension/ai/behaviour_factory.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ActionNode::ActionNode() : status_(ActionNodeStatus::ENTER), need_quit_(false) {}
ActionNode::~ActionNode() {}

bool ActionNode::Initialize(BehaviourNode *parent, TiXmlElement *node) {
  if(BehaviourNode::Initialize(parent, node) == false) {
    global::LogError("%s:%d (%s) 初始化 BehaviourNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->status_ = ActionNodeStatus::ENTER;
  this->need_quit_ = false;
  return this->LoadConfigure(node);
}

void ActionNode::Deallocate() {
  BehaviourNode::Deallocate();

  ActionAllocator *allocator =
    ActionFactory::GetInstance()->GetAllocator(this->GetTypeName());
  if(allocator == NULL) {
    LOG_ERROR("获取 ActionAllocator 失败 type(%s)", this->GetTypeName().c_str());
    return ;
  }

  allocator->Deallocate(this);
}

void ActionNode::OnTransition(BlackBoard *black_board) {
  if(this->need_quit_) {
    this->OnQuit(black_board);
    this->need_quit_ = false;
  }
  this->status_ = ActionNodeStatus::ENTER;
}

RunningStatus::type ActionNode::OnRunning(BlackBoard *black_board) {
  RunningStatus::type ret = RunningStatus::FINISH;

  if(this->status_ == ActionNodeStatus::ENTER) {
    this->OnEnter(black_board);
    this->status_ = ActionNodeStatus::EXECUTE;
    ret = RunningStatus::EXECUTING;
    this->need_quit_ = true;
  }
  if(this->status_ == ActionNodeStatus::EXECUTE) {
    ret = this->OnExecute(black_board);
    if(ret == RunningStatus::FINISH) {
      this->status_ = ActionNodeStatus::QUIT;
    }
  }
  if(this->status_ == ActionNodeStatus::QUIT) {
    this->OnQuit(black_board);
    this->need_quit_ = false;
    this->status_ = ActionNodeStatus::ENTER;
  }

  return ret;
}

}  // namespace ai

}  // namespace server

}  // namespace game

