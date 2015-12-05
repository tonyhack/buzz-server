//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 14:32:00.
// File name: team_actor_pool.cc
//
// Description:
// Define class TeamActorPool.
//

#include "session_server/server/extension/team/team_actor_pool.h"

#include "session_server/server/extension/team/team_actor.h"

namespace session {

namespace server {

namespace team {

bool TeamActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void TeamActorPool::Finalize() {
  this->actors_.Finalize();
}

TeamActor *TeamActorPool::Allocate() {
  return this->actors_.Allocate();
}

void TeamActorPool::Deallocate(TeamActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace team

}  // namespace server

}  // namespace session

