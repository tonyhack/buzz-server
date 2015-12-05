//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-09 12:28:01.
// File name: simple_use_skill_action_allocator.cc
//
// Description:
// Define class SimpleUseSkillActionAllocator.
//

#include "game_server/server/extension/ai/simple_use_skill_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

SimpleUseSkillActionAllocator::SimpleUseSkillActionAllocator() {}
SimpleUseSkillActionAllocator::~SimpleUseSkillActionAllocator() {}

SimpleUseSkillAction *SimpleUseSkillActionAllocator::Allocate(
    BehaviourNode *parent, TiXmlElement *node) {
  // 分配
  SimpleUseSkillAction *action = new (std::nothrow) SimpleUseSkillAction();
  if(action == NULL) {
    global::LogError("%s:%d (%s) 分配 SimpleUseSkillAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  // 初始化
  if(action->Initialize(parent, node) == false) {
    delete action;
    global::LogError("%s:%d (%s) 初始化 SimpleUseSkillAction 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return NULL;
  }
  return action;
}

void SimpleUseSkillActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game


