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

#include "game_server/server/extension/playing/playing_manager.h"

#include "game_server/server/extension/playing/playing.h"

namespace game {

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

bool PlayingManager::AddScenePlaying(core::uint64 scene, core::uint64 playing) {
  ScenePlayingHashmap::iterator iterator = this->scene_playings_.find(scene);
  if(iterator != this->scene_playings_.end()) {
    return false;
  }

  this->scene_playings_.insert(std::make_pair(scene, playing));

  return true;
}

core::uint64 PlayingManager::RemoveScenePlaying(core::uint64 scene) {
  core::uint64 playing = 0;

  ScenePlayingHashmap::iterator iterator = this->scene_playings_.find(scene);
  if(iterator != this->scene_playings_.end()) {
    playing = iterator->second;
    this->scene_playings_.erase(iterator);
  }

  return playing;
}

core::uint64 PlayingManager::GetScenePlaying(core::uint64 scene) {
  ScenePlayingHashmap::iterator iterator = this->scene_playings_.find(scene);
  if(iterator != this->scene_playings_.end()) {
    return iterator->second;
  }

  return 0;
}

}  // namespace playing

}  // namespace server

}  // namespace game

