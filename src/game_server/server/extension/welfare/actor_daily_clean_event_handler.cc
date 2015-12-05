#include "game_server/server/extension/welfare/actor_daily_clean_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

ActorDailyCleanEventHandler::ActorDailyCleanEventHandler() {}
ActorDailyCleanEventHandler::~ActorDailyCleanEventHandler() {}

bool ActorDailyCleanEventHandler::Initialize() {
  // Follow EVENT_ACTOR_DAILY_CLEAN
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_DAILY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorDailyCleanEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorDailyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorDailyCleanEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventActorDailyClean) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  event::EventActorDailyClean *event = (event::EventActorDailyClean *)message;
  if(event->__isset.actor_ == true) {
    // 清理单个玩家
    WelfareActor *welfare_actor = WelfareActorManager::GetInstance()->Get(event->actor_);
    if(welfare_actor == NULL) {
      LOG_ERROR("WelfareActor(%lu) not exists.", event->actor_);
      return ;
    }
    welfare_actor->DailyClean(event->days_);
  } else {
    // 清理所有玩家
    WelfareActorManager::GetInstance()->DailyClean(1);
  }
}

}  // namespace welfare

}  // namespace server

}  // namespace game

