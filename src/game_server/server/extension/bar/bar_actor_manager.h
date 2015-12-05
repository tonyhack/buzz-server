//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-30 18:52:57.
// File name: bar_actor_manager.h
//
// Description:
//

#ifndef __GAME__SERVER__BAR__BAR__ACTOR__MANAGER__H
#define __GAME__SERVER__BAR__BAR__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "game_server/server/extension/bar/bar_actor.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace bar {

class BarActor;

class BarActorManager : public global::SingletonFactory<BarActorManager> {
  friend class global::SingletonFactory<BarActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, BarActor *> ActorHashmap;

 public:
  BarActor *Get(core::uint64 id);
  bool Add(BarActor *actor);
  BarActor *Remove(core::uint64 id);
  
 private:
  BarActorManager() {}
  ~BarActorManager() {}

  ActorHashmap actors_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__BAR__ACTOR__MANAGER__H

