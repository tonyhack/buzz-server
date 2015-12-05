//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 12:01:24.
// File name: team_actor.cc
//
// Description:
// Define class TeamActor.
//

#include "game_server/server/extension/team/team_actor.h"

namespace game {

namespace server {

namespace team {

TeamActor::TeamActor() {}
TeamActor::~TeamActor() {}

bool TeamActor::Initialize(GameActor *actor) {
  if(actor == NULL) {
    return false;
  }

  this->actor_ = actor;
  this->SetTeam(0);
  this->team_buff_ = 0;

  return true;
}

void TeamActor::Finalize() {}

}  // namespace team

}  // namespace server

}  // namespace game

