#include "session_server/server/extension/guild/actor_weekly_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

ActorWeeklyCleanEventHandler::ActorWeeklyCleanEventHandler() {}
ActorWeeklyCleanEventHandler::~ActorWeeklyCleanEventHandler() {}

bool ActorWeeklyCleanEventHandler::Initialize() {
  // Follow 所有玩家每周清理事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_WEEKLY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorWeeklyCleanEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorWeeklyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorWeeklyCleanEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorWeeklyClean) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorWeeklyClean *event = (event::EventActorWeeklyClean *)message;

  if (event->__isset.actor_) {
    GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_);
    if (NULL == guild_actor) {
      LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_);
      return;
    }
    guild_actor->WeeklyClean();
  } else {
    GuildActorManager::GetInstance()->WeeklyClean();
  }
}

}  // namespace guild

}  // namespace server

}  // namespace session

