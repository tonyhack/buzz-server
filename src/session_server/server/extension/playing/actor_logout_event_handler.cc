//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 11:12:44.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "session_server/server/extension/playing/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/playing/playing.h"
#include "session_server/server/extension/playing/playing_actor.h"
#include "session_server/server/extension/playing/playing_actor_manager.h"
#include "session_server/server/extension/playing/playing_actor_pool.h"
#include "session_server/server/extension/playing/playing_manager.h"
#include "session_server/server/extension/playing/playing_team.h"
#include "session_server/server/extension/playing/playing_team_manager.h"

namespace session {

namespace server {

namespace playing {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLogoutEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message, size, channel_type 可能错误");
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 获取 PlayingActor 对象
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->id_);
  if(playing_actor == NULL) {
    LOG_ERROR("获取 PlayingActor(%lu)对象 失败", event->id_);
    return ;
  }

  PlayingTeamActor *team_actor = playing_actor->GetPlayingTeamActor();
  if (team_actor == NULL) {
    LOG_ERROR("获取 PlayingTeamActor(%lu) 失败", event->id_);
    return;
  }

  // 有副本队伍时，移除队员
  if (team_actor->GetTeam() > 0) {
    PlayingTeam *team =
      PlayingTeamManager::GetInstance()->Get(team_actor->GetTeam());
    if (team != NULL) {
      team->RemoveMember(team_actor);
    }
  }

  // 移除 PlayingActor 对象
  playing_actor = PlayingActorManager::GetInstance()->Remove(event->id_);
  if(playing_actor == NULL) {
    LOG_ERROR("玩家(%lu) 移除 PlayingActor对象 失败", event->id_);
    return ;
  }

  // 回收对象
  playing_actor->Finalize();
  PlayingActorPool::GetInstance()->Deallocate(playing_actor);
}

}  // namespace playing

}  // namespace server

}  // namespace session

