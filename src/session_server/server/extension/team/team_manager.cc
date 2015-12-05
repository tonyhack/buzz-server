//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 13:58:54.
// File name: team_manager.cc
//
// Description:
// Define class TeamManager.
//

#include "session_server/server/extension/team/team_manager.h"

#include "session_server/server/extension/team/team.h"

namespace session {

namespace server {

namespace team {

bool TeamManager::Add(Team *team) {
  if(team == NULL) {
    return false;
  }
  TeamHashmap::iterator iterator = this->teams_.find(team->GetID());
  if(iterator != this->teams_.end()) {
    return false;
  }
  this->teams_.insert(std::make_pair(team->GetID(), team));
  return true;
}

Team *TeamManager::Remove(core::uint64 id) {
  Team *team = NULL;
  TeamHashmap::iterator iterator = this->teams_.find(id);
  if(iterator != this->teams_.end()) {
    team = iterator->second;
    this->teams_.erase(iterator);
  }
  return team;
}

Team *TeamManager::Get(core::uint64 id) {
  TeamHashmap::iterator iterator = this->teams_.find(id);
  if(iterator != this->teams_.end()) {
    return iterator->second;
  }
  return NULL;
}

}  // namespace team

}  // namespace server

}  // namespace session

