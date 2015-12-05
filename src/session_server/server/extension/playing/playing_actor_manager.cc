//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 09:53:45.
// File name: playing_actor_manager.cc
//
// Description:
// Define class PlayingActorManager.
//

#include "session_server/server/extension/playing/playing_actor_manager.h"

#include "session_server/server/extension/playing/playing_actor.h"

namespace session {

namespace server {

namespace playing {

bool PlayingActorManager::Add(PlayingActor *actor) {
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

PlayingActor *PlayingActorManager::Remove(core::uint64 id) {
  PlayingActor *actor = NULL;

  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }

  return actor;
}

PlayingActor *PlayingActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }

  return NULL;
}

}  // namespace playing

}  // namespace server

}  // namespace session

