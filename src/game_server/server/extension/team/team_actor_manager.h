//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 13:34:39.
// File name: team_actor_manager.h
//
// Description:
// Define class TeamActorManager.
//

#ifndef __GAME__SERVER__TEAM__TEAM__ACTOR__MANAGER__H
#define __GAME__SERVER__TEAM__TEAM__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace team {

class TeamActor;

class TeamActorManager : public global::SingletonFactory<TeamActorManager> {
  friend class global::SingletonFactory<TeamActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, TeamActor *> ActorHashmap;

 public:
  TeamActor *Get(core::uint64 id);
  bool Add(TeamActor *actor);
  TeamActor *Remove(core::uint64 id);

 private:
  TeamActorManager() {}
  ~TeamActorManager() {}

  ActorHashmap actors_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__TEAM__ACTOR__MANAGER__H

