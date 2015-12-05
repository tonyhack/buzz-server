//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 15:37:59.
// File name: soul_actor_manager.h
//
// Description:
// Define class SoulActorManager.
//

#ifndef __GAME__SERVER__SOUL__SOUL__ACTOR__MANAGER__H
#define __GAME__SERVER__SOUL__SOUL__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class SoulActor;

class SoulActorManager : public global::SingletonFactory<SoulActorManager> {
  friend class global::SingletonFactory<SoulActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SoulActor *> ActorHashmap;

 public:
  SoulActor *Get(core::uint64 id);
  bool Add(SoulActor *actor);
  SoulActor *Remove(core::uint64 id);

  void DailyClean();

 private:
  SoulActorManager() {}
  ~SoulActorManager() {}

  ActorHashmap actors_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ACTOR__MANAGER__H

