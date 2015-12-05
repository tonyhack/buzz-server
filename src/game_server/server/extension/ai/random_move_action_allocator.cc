//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 13:57:44.
// File name: random_move_action_allocator.cc
//
// Description:
// Define class RandomMoveActionAllocator.
//

#include "game_server/server/extension/ai/random_move_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

RandomMoveActionAllocator::RandomMoveActionAllocator() {}
RandomMoveActionAllocator::~RandomMoveActionAllocator() {}

RandomMoveAction *RandomMoveActionAllocator::Allocate(BehaviourNode *parent,
    TiXmlElement *node) {
  // 分配
  RandomMoveAction *action = new (std::nothrow) RandomMoveAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 RandomMoveAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 RandomMoveAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }

  return action;
}

void RandomMoveActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

