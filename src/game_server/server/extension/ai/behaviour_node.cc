//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-08 17:31:18.
// File name: behaviour_node.cc
//
// Description:
// Define class BehaviourNode.
//

#include "game_server/server/extension/ai/behaviour_node.h"

#include "game_server/server/extension/ai/behaviour_factory.h"

namespace game {

namespace server {

namespace ai {

BehaviourNode::BehaviourNode() : parent_(NULL), running_status_(RunningStatus::FINISH) {}
BehaviourNode::~BehaviourNode() {}

bool BehaviourNode::Initialize(BehaviourNode *parent, TiXmlElement *node) {
  this->parent_ = parent;
  return true;
}

void BehaviourNode::Deallocate() {
  // 循环中先删除子结点，然后再对子结点进行Deallocate.
  while(true) {
    NodeVector::iterator iterator = this->children_.begin();
    if(iterator == this->children_.end()) {
      break;
    }
    BehaviourNode *node = *iterator;
    this->children_.erase(iterator);
    if(node) {
      node->Deallocate();
    }
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game

