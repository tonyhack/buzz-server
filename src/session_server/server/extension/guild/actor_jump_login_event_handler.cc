#include "session_server/server/extension/guild/actor_jump_login_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

ActorJumpLoginEventHandler::ActorJumpLoginEventHandler() {}
ActorJumpLoginEventHandler::~ActorJumpLoginEventHandler() {}

bool ActorJumpLoginEventHandler::Initialize() {
  // Follow 所有玩家跳场景登陆开始
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_JUMP_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorJumpLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorJumpLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorJumpLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorJumpLogin) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorJumpLogin *event = (event::EventActorJumpLogin *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(event->actor_);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", event->actor_);
    return;
  }

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_);
    return;
  }
  guild_actor->SendInitToGameServer();
}

}  // namespace guild

}  // namespace server

}  // namespace session

