//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 18:37:36.
// File name: cooling_role_pool.cc
//
// Description:
// Define class CoolingRolePool.
//

#include "game_server/server/extension/cooling/cooling_role_pool.h"

#include "game_server/server/extension/cooling/cooling_role.h"

namespace game {

namespace server {

namespace cooling {

bool CoolingRolePool::Initialize(size_t initial_number,
    size_t extend_number) {
  return this->roles_.Initialize(initial_number, extend_number);
}

void CoolingRolePool::Finalize() {
  this->roles_.Finalize();
}

CoolingRole *CoolingRolePool::Allocate() {
  return this->roles_.Allocate();
}

void CoolingRolePool::Deallocate(CoolingRole *role) {
  this->roles_.Deallocate(role);
}

}  // namespace cooling

}  // namespace server

}  // namespace game

