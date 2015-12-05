//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 11:42:14.
// File name: team_pool.h
//
// Description:
// Define class TeamPool.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__POOL__H
#define __SESSION__SERVER__TEAM__TEAM__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace team {

class Team;

class TeamPool : public global::SingletonFactory<TeamPool> {
  friend class global::SingletonFactory<TeamPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Team *Allocate();
  void Deallocate(Team *team);

 private:
  TeamPool() {}
  ~TeamPool() {}

  global::PoolTemplate<Team> teams_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__POOL__H

