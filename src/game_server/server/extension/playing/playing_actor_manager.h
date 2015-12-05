//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 09:49:03.
// File name: playing_actor_manager.h
//
// Description:
// Define class PlayingActorManager.
//

#ifndef __GAME__SERVER__PLAYING__PLAYING__ACTOR__MANAGER__H
#define __GAME__SERVER__PLAYING__PLAYING__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/playing_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class PlayingActor;

class PlayingActorManager : public global::SingletonFactory<PlayingActorManager> {
  friend class global::SingletonFactory<PlayingActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, PlayingActor *> ActorHashmap;

 public:
  bool Add(PlayingActor *actor);
  PlayingActor *Remove(core::uint64 id);
  PlayingActor *Get(core::uint64 id);

  void PeriodClean(entity::PlayingPeriodType::type period_type);
  void ResetMaze();

 private:
  PlayingActorManager() {}
  ~PlayingActorManager() {}

  ActorHashmap actors_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__ACTOR__MANAGER__H

