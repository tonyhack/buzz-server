//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 15:21:40.
// File name: condition_node.cc
//
// Description:
// Define class ConditionNode.
//

#include "game_server/server/extension/ai/condition_node.h"

#include "game_server/server/extension/ai/behaviour_factory.h"
#include "game_server/server/extension/ai/condition_allocator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

ConditionNode::ConditionNode() : negation_(false) {}
ConditionNode::~ConditionNode() {}

bool ConditionNode::Initialize(BehaviourNode *parent, TiXmlElement *node) {
  if(BehaviourNode::Initialize(parent, node) == false) {
    global::LogError("%s:%d (%s) 初始化 BehaviourNode 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  const char *str = node->Attribute("negation");
  if(strncmp(str, "true", sizeof("true")) == 0) {
    this->negation_ = true;
  } else {
    this->negation_ = false;
  }
  return this->LoadConfigure(node);
}

void ConditionNode::Deallocate() {
  BehaviourNode::Deallocate();

  ConditionAllocator *allocator =
    ConditionFactory::GetInstance()->GetAllocator(this->GetTypeName());
  if(allocator == NULL) {
    LOG_ERROR("获取 ConditionAllocator 失败 type(%s)", this->GetTypeName().c_str());
    return ;
  }

  allocator->Deallocate(this);
}

}  // namespace ai

}  // namespace server

}  // namespace game

