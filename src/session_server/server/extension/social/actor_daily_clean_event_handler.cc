#include "session_server/server/extension/social/actor_daily_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ActorDailyCleanEventHandler::ActorDailyCleanEventHandler() {}
ActorDailyCleanEventHandler::~ActorDailyCleanEventHandler() {}

bool ActorDailyCleanEventHandler::Initialize() {
  // Follow 所有玩家每日清理事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_DAILY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorDailyCleanEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorDailyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorDailyCleanEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorDailyClean) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorDailyClean *event = (event::EventActorDailyClean *)message;

  if (event->__isset.actor_ == false) {
    SocialActorManager::GetInstance()->DailyClean();
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

