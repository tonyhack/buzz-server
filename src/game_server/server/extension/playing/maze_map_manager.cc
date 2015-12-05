#include "game_server/server/extension/playing/maze_map_manager.h"

#include "global/logging.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_playing_protocol_types.h"

namespace game {

namespace server {

namespace playing {

MazeMapManager::MazeMapManager() : last_update_time_(0) {}
MazeMapManager::~MazeMapManager() {}

bool MazeMapManager::Initialize() {
  return true;
}

void MazeMapManager::Finalize() {
  this->last_update_time_ = 0;
  this->maps_.clear();
}

void MazeMapManager::Update(const session::protocol::MessageMazeMapInfoSynchronize &sync) {
  this->maps_.clear();
  for (size_t i = 0; i < sync.maps_.size(); ++i) {
    this->maps_.push_back(sync.maps_[i].grids_);
  }
  this->last_update_time_ = sync.last_update_time_;
}

const MazeMapManager::MazeGridVector *MazeMapManager::GetMap(core::int32 floor) const {
  if (floor > (core::int32)this->maps_.size() || floor < 1) {
    return NULL;
  }
  return &this->maps_[floor - 1];
}

}  // namespace playing

}  // namespace server

}  // namespace game

