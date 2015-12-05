//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-21 16:59:53.
// File name: condition.cc
//
// Description:
// Define class Condition.
//

#include "game_server/server/extension/buff/condition.h"

#include "game_server/server/extension/buff/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

Condition::Condition() {}
Condition::~Condition() {}

bool Condition::Initialize(core::int32 id, BuffRole *role) {
  const BuffConditionCell *cell = Configure::GetInstance()->GetConditionCell(id);
  if(cell == NULL) {
    LOG_ERROR("获取 BuffConditionCell(%d) 失败", id);
    return false;
  }

  this->id_ = id;
  this->param1_ = cell->param1_;
  this->passed_ = false;

  return true;
}

void Condition::Finalize() {
  this->id_ = 0;
  this->param1_ = 0;
  this->passed_ = false;
}

}  // namespace buff

}  // namespace server

}  // namespace game

