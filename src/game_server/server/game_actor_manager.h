//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:17:50.
// File name: game_actor_manager.h
//
// Description:
// Define class GameActorManager.
//

#ifndef __GAME__SERVER__GAME__ACTOR__MANAGER__H
#define __GAME__SERVER__GAME__ACTOR__MANAGER__H

#include <map>
#include <set>
#include <string>

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameActor;

class GameActorManager : public global::SingletonFactory<GameActorManager> {
 friend class global::SingletonFactory<GameActorManager>;

 public:
  typedef __gnu_cxx::hash_map<core::uint64, GameActor *> ActorHashmap;
  typedef __gnu_cxx::hash_map<std::string, core::uint64,
          global::hash<std::string> > ActorIDHashmap;

  GameActor *GetActor(core::uint64 id);
  GameActor *GetActor(const std::string &name);

  bool AddActor(GameActor *actor);
  GameActor *RemoveActor(core::uint64 id);

  void KickActor(core::uint64 id);
  void KickActors();

  inline const ActorHashmap &GetActors() const {
    return this->actors_;
  }

  void DailyClean(core::int32 days);

 private:
  GameActorManager() {}
  ~GameActorManager() {}

  ActorHashmap actors_;
  ActorIDHashmap ids_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__ACTOR__MANAGER__H

