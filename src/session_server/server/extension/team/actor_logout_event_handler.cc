//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-04 16:23:42.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "session_server/server/extension/team/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/team/team.h"
#include "session_server/server/extension/team/team_actor.h"
#include "session_server/server/extension/team/team_actor_manager.h"
#include "session_server/server/extension/team/team_actor_pool.h"
#include "session_server/server/extension/team/team_manager.h"

namespace session {

namespace server {

namespace team {

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

  // 获取 TeamActor 对象
  TeamActor *team_actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(team_actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu)对象 失败");
    return ;
  }

  // 获取玩家队伍ID
  core::uint64 team_id = team_actor->GetTeamID();
  if(team_id != 0) {
    // 队伍存在时，把自己从队伍中踢出
    Team *team = TeamManager::GetInstance()->Get(team_id);
    if(team != NULL) {
      team->RemoveMember(team_actor);
    } else {
      LOG_ERROR("获取玩家(%lu) 所在队伍(%lu) 失败");
    }
  }

  // 移除 TeamActor 对象
  team_actor = TeamActorManager::GetInstance()->Remove(event->id_);
  if(team_actor == NULL) {
    LOG_ERROR("玩家(%lu) 移除 TeamActor对象 失败", event->id_);
    return ;
  }

  // 回收对象
  team_actor->Finalize();
  TeamActorPool::GetInstance()->Deallocate(team_actor);
}

}  // namespace team

}  // namespace server

}  // namespace session

