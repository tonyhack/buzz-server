//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-19 09:08:26.
// File name: actor_change_fight_score_event_handler.cc
//
// Description:
// Define class ActorChangeFightScoreEventHandler.
//

#include "session_server/server/extension/playing/actor_change_fight_score_event_handler.h"

#include <boost/bind.hpp>

#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_team.h"
#include "session_server/server/extension/playing/playing_team_manager.h"

namespace session {

namespace server {

namespace playing {

ActorChangeFightScoreEventHandler::ActorChangeFightScoreEventHandler() {}
ActorChangeFightScoreEventHandler::~ActorChangeFightScoreEventHandler() {}

bool ActorChangeFightScoreEventHandler::Initialize() {
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_CHANGE_FIGHT_SCORE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorChangeFightScoreEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorChangeFightScoreEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorChangeFightScoreEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorChangeFightScore) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorChangeFightScore *event = (event::EventActorChangeFightScore *)message;

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_id_);
  if (playing_actor == NULL) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", event->actor_id_);
    return;
  }

  PlayingTeamActor *team_actor = playing_actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("Get PlayingTeamActor(%lu) failed.", event->actor_id_);
    return;
  }

  core::uint32 team_id = team_actor->GetTeam();
  if (team_id <= 0) {
    return;
  }

  PlayingTeam *team = PlayingTeamManager::GetInstance()->Get(team_id);
  if (team == NULL) {
    LOG_ERROR("Get PlayingTeam(%lu) failed.", team_id);
    return;
  }

  SessionActor *session_actor = playing_actor->GetActor();
  if (session_actor == NULL) {
    LOG_ERROR("Get SessionActor(%lu) failed.", event->actor_id_);
    return;
  }

  BriefActor *brief_actor = session_actor->GetBriefActor();
  if (brief_actor == NULL) {
    LOG_ERROR("Get BriefActor(%lu) failed.", event->actor_id_);
  }

  gateway::protocol::MessagePlayingTeamMemberSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(event->actor_id_));
  synchronize.__set_fight_score_(brief_actor->GetCurrentFightScore());
  team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_MEMBER_SYNCHRONIZE);
}

}  // namespace playing

}  // namespace server

}  // namespace session

