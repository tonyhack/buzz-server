//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 00:33:02.
// File name: cooling_group_pool.cc
//
// Description:
//

#include "game_server/server/extension/cooling/cooling_group_pool.h"

#include "game_server/server/extension/cooling/cooling_group.h"

namespace game {

namespace server {

namespace cooling {

bool CoolingGroupPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->groups_.Initialize(initial_number, extend_number);
}

void CoolingGroupPool::Finalize() {
  this->groups_.Finalize();
}

CoolingGroup *CoolingGroupPool::Allocate() {
  return this->groups_.Allocate();
}

void CoolingGroupPool::Deallocate(CoolingGroup *group) {
  return this->groups_.Deallocate(group);
}

}  // namespace cooling

}  // namespace server

}  // namespace cooling

