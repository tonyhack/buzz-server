//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 11:56:15.
// File name: team_pool.cc
//
// Description:
// Define class TeamPool.
//

#include "session_server/server/extension/team/team_pool.h"

#include "session_server/server/extension/team/team.h"

namespace session {

namespace server {

namespace team {

bool TeamPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->teams_.Initialize(initial_number, extend_number);
}

void TeamPool::Finalize() {
  this->teams_.Finalize();
}

Team *TeamPool::Allocate() {
  return  this->teams_.Allocate();
}

void TeamPool::Deallocate(Team *team) {
  this->teams_.Deallocate(team);
}

}  // namespace team

}  // namespace server

}  // namespace session

