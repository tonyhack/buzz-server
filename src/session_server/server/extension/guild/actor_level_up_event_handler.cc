#include "session_server/server/extension/guild/actor_level_up_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

ActorLevelUpEventHandler::ActorLevelUpEventHandler() {}
ActorLevelUpEventHandler::~ActorLevelUpEventHandler() {}

bool ActorLevelUpEventHandler::Initialize() {
  // Follow 所有玩家升级事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LEVEL_UP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorLevelUpEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLevelUpEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorLevelUpEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorLevelUp) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }
  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;

  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_id_);
    return;
  }
  guild_actor->SendLevelSync();
}

}  // namespace guild

}  // namespace server

}  // namespace session
