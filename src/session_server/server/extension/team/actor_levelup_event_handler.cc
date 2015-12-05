//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 18:26:59.
// File name: actor_levelup_event_handler.cc
//
// Description:
// Define class ActorLevelupEventHandler.
//

#include "session_server/server/extension/team/actor_levelup_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "global/common_functions.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_actor_pool.h"
#include "session_server/server/extension/team/team_manager.h"

namespace session {

namespace server {

namespace team {

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
  if(message == NULL || sizeof(event::EventActorLevelUp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->actor_id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->actor_id_);
    return ;
  }

  // 玩家是否有组队
  if(actor->GetTeamID() == 0) {
    return ;
  }

  // 得到玩家组队对象
  Team *team = TeamManager::GetInstance()->Get(actor->GetTeamID());
  if(team == NULL) {
    LOG_ERROR("获取 Team(%lu) 失败", actor->GetTeamID());
    return ;
  }

  gateway::protocol::MessageTeamMemberSynchronize synchronize;
  synchronize.__set_actor_(global::ToString(event->actor_id_));
  synchronize.__set_level_(actor->GetLevel());

  team->BroadcastMessage(synchronize,
      gateway::protocol::MessageType::MESSAGE_TEAM_MEMBER_SYNCHRONIZE);
}

}  // namespace team

}  // namespace server

}  // namespace session

