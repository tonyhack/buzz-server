//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-05-22 04:42:01.
// File name: actor_levelup_event_handler.cc
//
// Description:
// Define class ActorLevelupEventHandler.
//

#include "session_server/server/extension/playing/actor_levelup_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/common_functions.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_team.h"
#include "session_server/server/extension/playing/playing_team_actor.h"
#include "session_server/server/extension/playing/playing_team_manager.h"

namespace session {

namespace server {

namespace playing {

ActorLevelupEventHandler::ActorLevelupEventHandler() {}
ActorLevelupEventHandler::~ActorLevelupEventHandler() {}

bool ActorLevelupEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LEVEL_UP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLevelupEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLevelupEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLevelupEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventActorLevelUp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_id_);
  if (playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu) 失败", event->actor_id_);
    return;
  }

  SessionActor *session_actor = playing_actor->GetActor();
  if (session_actor == NULL) {
    LOG_ERROR("获取 SessionActor(%lu) 失败", event->actor_id_);
    return;
  }

  PlayingTeamActor *team_actor = playing_actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", event->actor_id_);
    return;
  }

  core::uint64 team_id = team_actor->GetTeam();
  if (team_id == 0) {
    return;
  }

  PlayingTeam *team = PlayingTeamManager::GetInstance()->Get(team_id);
  if (team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", team_id);
    return;
  }

  gateway::protocol::MessagePlayingTeamMemberSynchronize synchronize;
  synchronize.__set_actor_(session_actor->GetStringID());
  synchronize.__set_level_(session_actor->GetLevel());

  team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_PLAYING_TEAM_MEMBER_SYNCHRONIZE);
}

}  // namespace playing

}  // namespace server

}  // namespace session

