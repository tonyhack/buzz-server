//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:26:13.
// File name: buff_role_pool.cc
//
// Description:
// Define class BuffRolePool.
//

#include "game_server/server/extension/buff/buff_role_pool.h"

#include "game_server/server/extension/buff/buff_role.h"

namespace game {

namespace server {

namespace buff {

bool BuffRolePool::Initialize(size_t initial_number,
    size_t extend_number) {
  return this->roles_.Initialize(initial_number, extend_number);
}

void BuffRolePool::Finalize() {
  this->roles_.Finalize();
}

BuffRole *BuffRolePool::Allocate() {
  return this->roles_.Allocate();
}

void BuffRolePool::Deallocate(BuffRole *role) {
  this->roles_.Deallocate(role);
}

}  // namespace buff

}  // namespace server

}  // namespace game

