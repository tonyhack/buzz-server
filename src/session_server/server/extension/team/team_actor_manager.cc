//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 13:50:51.
// File name: team_actor_manager.cc
//
// Description:
// Define class TeamActorManager.
//

#include "session_server/server/extension/team/team_actor_manager.h"

#include "session_server/server/extension/team/team_actor.h"

namespace session {

namespace server {

namespace team {

bool TeamActorManager::Add(TeamActor *actor) {
  if(actor == NULL) {
    return false;
  }
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

TeamActor *TeamActorManager::Remove(core::uint64 id) {
  TeamActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

TeamActor *TeamActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

}  // namespace team

}  // namespace server

}  // namespace session

