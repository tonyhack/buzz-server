//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-02 22:46:36.
// File name: buff_pool.cc
//
// Description:
// Define class BuffPool.
//

#include "game_server/server/extension/buff/buff_pool.h"

#include "game_server/server/extension/buff/buff.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

bool BuffPool::Initialize(size_t initial_number, size_t extend_number) {
  if(this->buffs_.Initialize(initial_number, extend_number) == false) {
    global::LogError("%s:%d (%s) 初始化属性效果失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->index_ = 1;
  return true;
}

void BuffPool::Finalize() {
  this->buffs_.Finalize();
}

Buff *BuffPool::Allocate() {
  Buff * buff = this->buffs_.Allocate();
  if (buff) {
    buff->SetIndex(this->index_++);
  }
  return buff;
}

void BuffPool::Deallocate(Buff *buff) {
  this->buffs_.Deallocate(buff);
}

}  // namespace buff

}  // namespace server

}  // namespace game

