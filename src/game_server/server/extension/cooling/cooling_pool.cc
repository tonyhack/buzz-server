//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 00:20:13.
// File name: cooling_pool.cc
//
// Description:
// Define class CoolingPool.
//

#include "game_server/server/extension/cooling/cooling_pool.h"

#include "game_server/server/extension/cooling/cooling.h"

namespace game {

namespace server {

namespace cooling {

bool CoolingPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->coolings_.Initialize(initial_number, extend_number);
}

void CoolingPool::Finalize() {
  this->coolings_.Finalize();
}

Cooling *CoolingPool::Allocate() {
  return this->coolings_.Allocate();
}

void CoolingPool::Deallocate(Cooling *cooling) {
  this->coolings_.Deallocate(cooling);
}

}  // namespace cooling

}  // namespace server

}  // namespace game

