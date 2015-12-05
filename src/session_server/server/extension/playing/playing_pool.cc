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

#include "session_server/server/extension/playing/playing_pool.h"

#include "session_server/server/extension/playing/playing.h"

namespace session {

namespace server {

namespace playing {

bool PlayingPool::Initialize(size_t initial_number, size_t extend_number) {
  this->id_ = 0;
  return this->playings_.Initialize(initial_number, extend_number);
}

void PlayingPool::Finalize() {
  this->playings_.Finalize();
}

Playing *PlayingPool::Allocate() {
  Playing *playing = this->playings_.Allocate();
  if(playing) {
    playing->Allocate(++this->id_);
  }

  return playing;
}

void PlayingPool::Deallocate(Playing *playing) {
  if(playing) {
    playing->Deallocate();
    this->playings_.Deallocate(playing);
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

