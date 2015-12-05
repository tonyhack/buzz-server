//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-14 03:56:27.
// File name: playing_team_pool.h
//
// Description:
// Define class PlayingTeamPool.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__TEAM__POOL__H
#define __SESSION__SERVER__PLAYING__PLAYING__TEAM__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace playing {

class PlayingTeam;

class PlayingTeamPool : public global::SingletonFactory<PlayingTeamPool> {
 friend class global::SingletonFactory<PlayingTeamPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  PlayingTeam *Allocate();
  void Deallocate(PlayingTeam *team);

 private:
  PlayingTeamPool() {}
  ~PlayingTeamPool() {}

  global::PoolTemplate<PlayingTeam> teams_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__TEAM__POOL__H

