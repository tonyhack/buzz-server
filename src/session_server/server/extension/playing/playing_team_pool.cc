//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-14 04:00:43.
// File name: playing_team_pool.cc
//
// Description:
// Define class PlayingTeamPool.
//

#include "session_server/server/extension/playing/playing_team_pool.h"

#include "session_server/server/extension/playing/playing_team.h"

namespace session {

namespace server {

namespace playing {

bool PlayingTeamPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->teams_.Initialize(initial_number, extend_number);
}

void PlayingTeamPool::Finalize() {
  this->teams_.Finalize();
}

PlayingTeam *PlayingTeamPool::Allocate() {
  return this->teams_.Allocate();
}

void PlayingTeamPool::Deallocate(PlayingTeam *team) {
  this->teams_.Deallocate(team);
}

}  // namespace playing

}  // namespace server

}  // namespace session

