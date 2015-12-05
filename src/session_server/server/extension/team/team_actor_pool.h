//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 14:28:26.
// File name: team_actor_pool.h
//
// Description:
// Define class TeamActorPool.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__ACTOR__POOL__H
#define __SESSION__SERVER__TEAM__TEAM__ACTOR__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

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

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__ACTOR__POOL__H

