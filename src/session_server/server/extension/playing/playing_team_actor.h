//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 04:52:30.
// File name: playing_team_actor.h
//
// Description:
// Define class PlayingTeamActor.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__TEAM__ACTOR__H
#define __SESSION__SERVER__PLAYING__PLAYING__TEAM__ACTOR__H

#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace playing {

class PlayingActor;

class PlayingTeamActor : public core::Noncopyable {
 public:
  PlayingTeamActor();
  ~PlayingTeamActor();

  bool Initialize(PlayingActor *actor);
  void Finalize();

  const std::string GetName() const;
  core::uint64 GetID() const;

  inline core::uint64 GetTeam() const {
    return this->team_;
  }
  inline void SetTeam(core::uint64 team) {
    this->team_ = team;
  }

  inline bool GetReady() const {
    return this->ready_;
  }
  inline void SetReady(bool ready) {
    this->ready_ = ready;
  }

  inline PlayingActor *GetActor() {
    return this->actor_;
  }

 private:
  PlayingActor *actor_;
  core::uint64 team_;
  bool ready_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__TEAM__ACTOR__H

