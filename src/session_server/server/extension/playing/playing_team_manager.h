//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 06:52:06.
// File name: playing_team_manager.h
//
// Description:
// Define class PlayingTeamManager.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__TEAM__MANAGER__H
#define __SESSION__SERVER__PLAYING__PLAYING__TEAM__MANAGER__H

#include <map>
#include <set>

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingTeam;

class PlayingTeamManager : public global::SingletonFactory<PlayingTeamManager> {
  friend class global::SingletonFactory<PlayingTeamManager>;

  typedef __gnu_cxx::hash_map<core::uint64, PlayingTeam *> TeamHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, core::uint64> ReadyTeamHashmap;

 public:
  typedef std::set<core::uint64> TeamSet;
  typedef std::map<core::int32, TeamSet> PlayingTeamMap;

  bool Add(PlayingTeam *team);
  PlayingTeam *Remove(core::uint64 id);
  PlayingTeam *Get(core::uint64 id);

  const TeamSet *GetPlayingTeams(core::int32 playing_template) const;

  bool AddReadyTeam(PlayingTeam *team);
  void RemoveReadyTeam(core::uint64 playing);
  PlayingTeam *GetReadyTeam(core::uint64 playing);

 private:
  PlayingTeamManager() {}
  ~PlayingTeamManager() {}

  TeamHashmap teams_;

  PlayingTeamMap playing_teams_;
  ReadyTeamHashmap ready_teams_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__TEAM__MANAGER__H

