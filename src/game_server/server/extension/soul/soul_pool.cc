//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 11:17:55.
// File name: soul_pool.cc
//
// Description:
// Define class SoulPool.
//

#include "game_server/server/extension/soul/soul_pool.h"

#include "game_server/server/extension/soul/soul.h"

namespace game {

namespace server {

namespace soul {

bool SoulPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->souls_.Initialize(initial_number, extend_number);
}

void SoulPool::Finalize() {
  this->souls_.Finalize();
}

Soul *SoulPool::Allocate() {
  return this->souls_.Allocate();
}

void SoulPool::Deallocate(Soul *soul) {
  this->souls_.Deallocate(soul);
}

}  // namespace soul

}  // namespace server

}  // namespace game

