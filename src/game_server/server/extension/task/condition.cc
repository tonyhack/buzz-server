//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 14:14:24.
// File name: condition.cc
//
// Description:
// Define class Condition.
//

#include "game_server/server/extension/task/condition.h"

#include "game_server/server/extension/task/task.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

Condition::Condition() : task_(NULL) {}
Condition::~Condition() {}

bool Condition::Initialize(Task *task, core::uint32 param1, core::uint32 param2,
      core::uint32 value) {
  if(task == NULL) {
    global::LogError("%s:%d (%s) 初始化 Condition 时, 参数为空",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->task_ = task;

  return true;
}

void Condition::Finalize() {}

}  // namespace task

}  // namespace server

}  // namespace game

