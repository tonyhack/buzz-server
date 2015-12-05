//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-28 13:46:10.
// File name: team_actor.cc
//
// Description:
// Define class TeamActor.
//

#include "session_server/server/extension/team/team_actor.h"

#include "session_server/protocol/game_team_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace session {

namespace server {

namespace team {

TeamActor::TeamActor() {}
TeamActor::~TeamActor() {}

bool TeamActor::Initialize(SessionActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("参数 actor 为 NULL");
    return false;
  }

  if(this->commands_.Initialize() == false) {
    LOG_ERROR("初始化 TeamCommands 失败");
  }

  this->actor_ = actor;
  this->team_id_ = 0;

  this->current_hp_ = 0;
  this->max_hp_ = 0;
  this->current_mp_ = 0;
  this->max_mp_ = 0;

  return true;
}

void TeamActor::Finalize() {
  this->commands_.Finalize();
}

void TeamActor::SynchronizeBuff(core::uint32 buff) {
  if(this->actor_ == NULL) {
    return ;
  }
  protocol::MessageTeamBuffSynchronize message;
  message.__set_actor_(this->GetID());
  message.__set_buff_(buff);
  this->actor_->SendGameMessage(message,
      protocol::GameMessageType::MESSAGE_TEAM_BUFF_SYNCHRONIZE);
}

}  // namespace team

}  // namespace server

}  // namespace session

