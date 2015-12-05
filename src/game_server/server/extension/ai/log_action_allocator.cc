//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-12 17:58:31.
// File name: log_action_allocator.cc
//
// Description:
// Define class LogActionAllocator.
//

#include "game_server/server/extension/ai/log_action_allocator.h"

#include "global/logging.h"

namespace game {

namespace server {

namespace ai {

LogActionAllocator::LogActionAllocator() {}
LogActionAllocator::~LogActionAllocator() {}

LogAction *LogActionAllocator::Allocate(BehaviourNode *parent, TiXmlElement *node) {
  LogAction *action = new (std::nothrow) LogAction();
  if(action == NULL) {
    LOG_ERROR("分配 LogAction 失败");
    return NULL;
  }
  if(action->Initialize(parent, node) == false) {
    LOG_ERROR("初始化 LogAction 失败");
    return NULL;
  }
  return action;
}

void LogActionAllocator::Deallocate(BehaviourNode *node) {
  delete node;
}

}  // namespace ai

}  // namespace server

}  // namespace game

