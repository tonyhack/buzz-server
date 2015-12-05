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

#ifndef __SESSION__SERVER__PLAYING__PLAYING__MANAGER__H
#define __SESSION__SERVER__PLAYING__PLAYING__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class Playing;

class PlayingManager : public global::SingletonFactory<PlayingManager> {
  friend class global::SingletonFactory<PlayingManager>;

  typedef __gnu_cxx::hash_map<core::uint64, Playing *> PlayingHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, core::uint64> ActorPlayingHashmap;

 public:
  bool Add(Playing *playing);
  Playing *Remove(core::uint64 id);
  Playing *Get(core::uint64 id);

  bool AddActorPlaying(core::uint64 actor, core::uint64 playing);
  core::uint64 RemoveActorPlaying(core::uint64 actor);
  core::uint64 GetActorPlaying(core::uint64 actor);

 private:
  PlayingManager() {}
  ~PlayingManager() {}

  // 副本管理器
  PlayingHashmap playings_;

  // 玩家副本的对应关系
  ActorPlayingHashmap actor_playings_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__MANAGER__H

