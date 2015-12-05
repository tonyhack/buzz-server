//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-12 05:01:53.
// File name: playing_team_actor.cc
//
// Description:
// Define class PlayingTeamActor.
//

#include "session_server/server/extension/playing/playing_team_actor.h"

#include "session_server/server/extension/playing/playing_actor.h"

namespace session {

namespace server {

namespace playing {

PlayingTeamActor::PlayingTeamActor() {
}

PlayingTeamActor::~PlayingTeamActor() {
}

const std::string PlayingTeamActor::GetName() const {
  return this->actor_->GetName();
}

core::uint64 PlayingTeamActor::GetID() const {
  if (this->actor_) {
    return this->actor_->GetID();
  }
  return 0;
}

bool PlayingTeamActor::Initialize(PlayingActor *actor) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->actor_ = actor;
  this->team_ = 0;
  this->ready_ = false;

  return true;
}

void PlayingTeamActor::Finalize() {
  this->actor_ = NULL;
  this->team_ = 0;
  this->ready_ = false;
}

}  // namespace playing

}  // namespace server

}  // namespace session

