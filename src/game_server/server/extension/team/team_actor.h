//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 09:34:25.
// File name: team_actor.h
//
// Description:
// Define class TeamActor.
//

#ifndef __GAME__SERVER__TEAM__TEAM__ACTOR__H
#define __GAME__SERVER__TEAM__TEAM__ACTOR__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/game_actor.h"

namespace game {

namespace server {

namespace team {

class TeamActor : public core::Noncopyable {
 public:
  TeamActor();
  ~TeamActor();

  bool Initialize(GameActor *actor);
  void Finalize();

  // 获取/设置 队伍ID
  inline core::uint64 GetTeam() const {
    return this->team_;
  }
  inline void SetTeam(core::uint64 team) {
    this->team_ = team;
  }

  inline GameActor *GetActor() {
    return this->actor_;
  }

  inline core::uint64 GetID() const {
    if(this->actor_) {
      return this->actor_->GetID();
    }
    return 0;
  }

  inline core::uint64 GetTeamBuff() const {
    return this->team_buff_;
  }
  inline void SetTeamBuff(core::uint64 buff) {
    this->team_buff_ = buff;
  }

 private:
  GameActor *actor_;

  core::uint64 team_;

  core::uint32 team_buff_;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__TEAM__ACTOR__H

