//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 13:46:39.
// File name: team_actor_manager.h
//
// Description:
// Define class TeamActorManager.
//

#ifndef __SESSION__SERVER__TEAM__TEAM_ACTOR__MANAGER__H
#define __SESSION__SERVER__TEAM__TEAM_ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace team {

class TeamActor;

class TeamActorManager : public global::SingletonFactory<TeamActorManager> {
  friend class global::SingletonFactory<TeamActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, TeamActor *> ActorHashmap;

 public:
  bool Add(TeamActor *actor);
  TeamActor *Remove(core::uint64 id);
  TeamActor *Get(core::uint64 id);

 private:
  TeamActorManager() {}
  ~TeamActorManager() {}

  ActorHashmap actors_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM_ACTOR__MANAGER__H

