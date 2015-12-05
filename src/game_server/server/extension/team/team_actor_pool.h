//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 13:21:44.
// File name: team_actor_pool.h
//
// Description:
// Define class TeamActorPool.
//

#ifndef __GAME__SERVER__TEAM__TEAM__ACTOR__POOL__H
#define __GAME__SERVER__TEAM__TEAM__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace team {

class TeamActor;

class TeamActorPool : public global::SingletonFactory<TeamActorPool> {
  friend class global::SingletonFactory<TeamActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  TeamActor *Allocate();
  void Deallocate(TeamActor *actor);

 private:
  TeamActorPool() {}
  ~TeamActorPool() {}

  global::PoolTemplate<TeamActor> actors_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__TEAM__ACTOR__POOL__H

