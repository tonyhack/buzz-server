//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-30 18:34:05.
// File name: bar_actor_pool.cc
//
// Description:
//
#include "game_server/server/extension/bar/bar_actor_pool.h"

#include "game_server/server/extension/bar/bar_actor.h"

namespace game {

namespace server {

namespace bar {

bool BarActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->bars_.Initialize(initial_number, extend_number);
}

void BarActorPool::Finalize() {
  this->bars_.Finalize();
}

BarActor *BarActorPool::Allocate() {
  return this->bars_.Allocate();
}

void BarActorPool::Deallocate(BarActor *bar) {
  this->bars_.Deallocate(bar);
}

}  // namespace bar

}  // namespace server

}  // namespace game

