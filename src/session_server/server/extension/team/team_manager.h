//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-21 17:46:24.
// File name: team_manager.h
//
// Description:
// Define class TeamManager.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__MANAGER__H
#define __SESSION__SERVER__TEAM__TEAM__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace team {

class Team;

class TeamManager : public global::SingletonFactory<TeamManager> {
  friend class global::SingletonFactory<TeamManager>;

  typedef __gnu_cxx::hash_map<core::uint64, Team *> TeamHashmap;

 public:
  bool Add(Team *team);
  Team *Remove(core::uint64 id);
  Team *Get(core::uint64 id);

 private:
  TeamManager() {}
  ~TeamManager() {}

  TeamHashmap teams_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__MANAGER__H

