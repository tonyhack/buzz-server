//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 07:03:19.
// File name: playing_team_manager.cc
//
// Description:
// Define class PlayingTeamManager.
//

#include "session_server/server/extension/playing/playing_team_manager.h"

#include "session_server/server/extension/playing/playing_team.h"

namespace session {

namespace server {

namespace playing {

bool PlayingTeamManager::Add(PlayingTeam *team) {
  if (team == NULL) {
    return false;
  }

  TeamHashmap::const_iterator iterator = this->teams_.find(team->GetID());
  if (iterator != this->teams_.end()) {
    return false;
  }

  PlayingTeamMap::iterator iterator2 =
    this->playing_teams_.find(team->GetPlayingTemplate());
  if (iterator2 != this->playing_teams_.end()) {
    if (iterator2->second.find(team->GetID()) != iterator2->second.end()) {
      return false;
    }
    iterator2->second.insert(team->GetID());
  } else {
    TeamSet team_set;
    team_set.insert(team->GetID());
    this->playing_teams_.insert(std::make_pair(team->GetPlayingTemplate(), team_set));
  }

  this->teams_.insert(std::make_pair(team->GetID(), team));

  return true;
}

PlayingTeam *PlayingTeamManager::Remove(core::uint64 id) {
  PlayingTeam *team = NULL;

  TeamHashmap::iterator iterator = this->teams_.find(id);
  if (iterator != this->teams_.end()) {
    team = iterator->second;
    this->teams_.erase(iterator);

    PlayingTeamMap::iterator iterator2 = this->playing_teams_.find(team->GetPlayingTemplate());
    if (iterator2 != this->playing_teams_.end()) {
      iterator2->second.erase(team->GetID());
    }
  }

  return team;
}

PlayingTeam *PlayingTeamManager::Get(core::uint64 id) {
  TeamHashmap::iterator iterator = this->teams_.find(id);
  if (iterator != this->teams_.end()) {
    return iterator->second;
  }

  return NULL;
}

const PlayingTeamManager::TeamSet *PlayingTeamManager::GetPlayingTeams(
    core::int32 playing_template) const {
  PlayingTeamMap::const_iterator iterator = this->playing_teams_.find(playing_template);
  if (iterator != this->playing_teams_.end()) {
    return &iterator->second;
  }

  return NULL;
}

bool PlayingTeamManager::AddReadyTeam(PlayingTeam *team) {
  if (team == NULL || team->GetPlaying() == 0) {
    return false;
  }

  ReadyTeamHashmap::const_iterator iterator =
    this->ready_teams_.find(team->GetPlaying());
  if (iterator != this->ready_teams_.end()) {
    return false;
  }

  this->ready_teams_.insert(std::make_pair(team->GetPlaying(), team->GetID()));

  return true;
}

void PlayingTeamManager::RemoveReadyTeam(core::uint64 playing) {
  this->ready_teams_.erase(playing);
}

PlayingTeam *PlayingTeamManager::GetReadyTeam(core::uint64 playing) {
  ReadyTeamHashmap::const_iterator iterator = this->ready_teams_.find(playing);
  if (iterator == this->ready_teams_.end()) {
    return NULL;
  }

  return this->Get(iterator->second);
}

}  // namespace playing

}  // namespace server

}  // namespace session

