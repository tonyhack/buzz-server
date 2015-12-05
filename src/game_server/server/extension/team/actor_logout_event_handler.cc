//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 14:14:36.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "game_server/server/extension/team/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "game_server/server/extension/team/team_actor_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

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

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("%s:%d (%s) 参数 message 或 size 错误");
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  TeamActor *actor = TeamActorManager::GetInstance()->Remove(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu) 移除队伍玩家对象失败", event->id_);
    return ;
  }

  // 回收对象
  actor->Finalize();
  TeamActorPool::GetInstance()->Deallocate(actor);
}

}  // namespace team

}  // namespace server

}  // namespace game

