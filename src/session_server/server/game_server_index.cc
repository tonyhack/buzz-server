//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 15:08:24.
// File name: game_server_index.cc
//
// Description:
// Define class GameServerIndex.
//

#include "session_server/server/game_server_index.h"

namespace session {

namespace server {

GameServerIndex::GameServerIndex() {}
GameServerIndex::~GameServerIndex() {}

bool GameServerIndex::Add(core::uint32 id) {
  GameServerDataMap::const_iterator iterator = this->servers_.find(id);
  if(iterator != this->servers_.end()) {
    return false;
  }
  this->servers_.insert(std::make_pair(id, GameServerData()));
  return true;
}

bool GameServerIndex::Update(core::uint32 id, const GameServerData &data) {
  GameServerDataMap::iterator iterator = this->servers_.find(id);
  if(iterator == this->servers_.end()) {
    return false;
  }
  iterator->second = data;
  return true;
}

bool GameServerIndex::Remove(core::uint32 id) {
  GameServerDataMap::iterator iterator = this->servers_.find(id);
  if(iterator == this->servers_.end()) {
    return false;
  }
  this->servers_.erase(iterator);
  return true;
}

core::uint32 GameServerIndex::GetServer() const {
  if(this->servers_.empty() == true) {
    return core::kuint32max;
  }
  core::uint32 game_id = 0;
  core::uint32 number = 0;
  GameServerDataMap::const_iterator iterator = this->servers_.begin();
  for(; iterator != this->servers_.end(); ++iterator) {
    if(iterator->second.GetUserNumber() >= number) {
      game_id = iterator->first;
    }
  }
  return game_id;
}

}  // namespace server

}  // namespace session

