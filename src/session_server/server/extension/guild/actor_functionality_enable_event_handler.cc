#include "session_server/server/extension/guild/actor_functionality_enable_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "entity/functionality_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

ActorFunctionalityEnableEventHandler::ActorFunctionalityEnableEventHandler() {}
ActorFunctionalityEnableEventHandler::~ActorFunctionalityEnableEventHandler() {}

bool ActorFunctionalityEnableEventHandler::Initialize() {
  // Follow 所有玩家系统功能开启事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorFunctionalityEnableEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorFunctionalityEnableEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorFunctionalityEnableEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorFunctionalityEnable) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorFunctionalityEnable *event = (event::EventActorFunctionalityEnable *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(event->actor_id_);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", event->actor_id_);
    return;
  }
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_id_);
    return;
  }

  if (entity::FunctionalityType::GUILD_MODULE == event->functionality_id_) {
    if (guild_actor->EnableGuildFunc() == false) {
       LOG_ERROR("Actor(%lu) Enable GUILD_MODULE failed.", event->actor_id_);
       return;
    }
  }
}

}  // namespace guild

}  // namespace server

}  // namespace session

