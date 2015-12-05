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

#include "game_server/server/extension/playing/playing_actor_manager.h"

#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/maze_actor.h"

namespace game {

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

void PlayingActorManager::PeriodClean(entity::PlayingPeriodType::type period_type) {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->PeriodClean(period_type);
    }
  }
}

void PlayingActorManager::ResetMaze() {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->GetMazeActor()->EnterNextFloor(1);
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

