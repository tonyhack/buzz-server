//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 10:09:09.
// File name: playing_manager.h
//
// Description:
// Define class PlayingManager.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__MANAGER__H
#define __GAME__SERVER__PLAYING__PLAYING__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class Playing;

class PlayingManager : public global::SingletonFactory<PlayingManager> {
  friend class global::SingletonFactory<PlayingManager>;

  typedef __gnu_cxx::hash_map<core::uint64, Playing *> PlayingHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, core::uint64> ScenePlayingHashmap;

 public:
  bool Add(Playing *playing);
  Playing *Remove(core::uint64 id);
  Playing *Get(core::uint64 id);

  bool AddScenePlaying(core::uint64 scene, core::uint64 playing);
  core::uint64 RemoveScenePlaying(core::uint64 scene);
  core::uint64 GetScenePlaying(core::uint64 scene);

 private:
  PlayingManager() {}
  ~PlayingManager() {}

  PlayingHashmap playings_;
  ScenePlayingHashmap scene_playings_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__MANAGER__H

