//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:32:12.
// File name: playing_manager.cc
//
// Description:
// Define class PlayingManager.
//

#include "session_server/server/extension/playing/playing_manager.h"

#include "session_server/server/extension/playing/playing.h"

namespace session {

namespace server {

namespace playing {

bool PlayingManager::Add(Playing *playing) {
  if(playing == NULL) {
    return false;
  }

  PlayingHashmap::iterator iterator = this->playings_.find(playing->GetID());
  if(iterator != this->playings_.end()) {
    return false;
  }

  this->playings_.insert(std::make_pair(playing->GetID(), playing));

  return true;
}

Playing *PlayingManager::Remove(core::uint64 id) {
  Playing *playing = NULL;

  PlayingHashmap::iterator iterator = this->playings_.find(id);
  if(iterator != this->playings_.end()) {
    playing = iterator->second;
    this->playings_.erase(iterator);
  }

  return playing;
}

Playing *PlayingManager::Get(core::uint64 id) {
  PlayingHashmap::iterator iterator = this->playings_.find(id);
  if(iterator != this->playings_.end()) {
    return iterator->second;
  }

  return NULL;
}

bool PlayingManager::AddActorPlaying(core::uint64 actor, core::uint64 playing) {
  ActorPlayingHashmap::iterator iterator = this->actor_playings_.find(actor);
  if(iterator != this->actor_playings_.end()) {
    return false;
  }

  this->actor_playings_.insert(std::make_pair(actor, playing));

  return true;
}

core::uint64 PlayingManager::RemoveActorPlaying(core::uint64 actor) {
  core::uint64 playing = 0;

  ActorPlayingHashmap::iterator iterator = this->actor_playings_.find(actor);
  if(iterator != this->actor_playings_.end()) {
    playing = iterator->second;
    this->actor_playings_.erase(iterator);
  }

  return playing;
}

core::uint64 PlayingManager::GetActorPlaying(core::uint64 actor) {
  ActorPlayingHashmap::iterator iterator = this->actor_playings_.find(actor);
  if(iterator != this->actor_playings_.end()) {
    return iterator->second;
  }

  return 0;
}

}  // namespace playing

}  // namespace server

}  // namespace session

