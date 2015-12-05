//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:32:30.
// File name: playing_load_balance.cc
//
// Description:
// Define class PlayingLoadBalance.
//

#include "session_server/server/extension/playing/playing_load_balance.h"

namespace session {

namespace server {

namespace playing {

core::uint32 PlayingLoadBalance::Allocate() {
  PlayingPairVector::iterator allocate_iterator = this->playings_.begin();
  PlayingPairVector::iterator iterator = this->playings_.begin();
  for(; iterator != this->playings_.end(); ++iterator) {
    if(allocate_iterator->second > iterator->second) {
      allocate_iterator = iterator;
    }
  }

  if(allocate_iterator != this->playings_.end()) {
    ++allocate_iterator->second;
    return allocate_iterator->first;
  }

  return 0;
}

void PlayingLoadBalance::Deallocate(core::uint32 server_id) {
  PlayingPairVector::iterator iterator = this->playings_.begin();
  for(; iterator != this->playings_.end(); ++iterator) {
    if(iterator->first == server_id) {
      if(iterator->second > 0) {
        --iterator->second;
      }
      return ;
    }
  }
}

void PlayingLoadBalance::OnGameServerLogin(core::uint32 server_id) {
  PlayingPairVector::iterator iterator = this->playings_.begin();
  for(; iterator != this->playings_.end(); ++iterator) {
    if(iterator->first == server_id) {
      return ;
    }
  }

  this->playings_.push_back(PlayingPair(server_id, 0));
}

void PlayingLoadBalance::OnGameServerLogout(core::uint32 server_id) {
  PlayingPairVector::iterator iterator = this->playings_.begin();
  for(; iterator != this->playings_.end(); ++iterator) {
    if(iterator->first == server_id) {
      return ;
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session

