//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:04:25.
// File name: playing_pool.cc
//
// Description:
// Define class PlayingPool.
//

#include "game_server/server/extension/playing/playing_pool.h"

#include "game_server/server/extension/playing/playing.h"

namespace game {

namespace server {

namespace playing {

bool PlayingPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->playings_.Initialize(initial_number, extend_number);
}

void PlayingPool::Finalize() {
  this->playings_.Finalize();
}

Playing *PlayingPool::Allocate() {
  return this->playings_.Allocate();
}

void PlayingPool::Deallocate(Playing *playing) {
  this->playings_.Deallocate(playing);
}

}  // namespace playing

}  // namespace server

}  // namespace game

