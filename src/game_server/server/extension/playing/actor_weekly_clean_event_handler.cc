#include "game_server/server/extension/playing/actor_weekly_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"

namespace game {

namespace server {

namespace playing {

ActorWeeklyCleanEventHandler::ActorWeeklyCleanEventHandler() {}
ActorWeeklyCleanEventHandler::~ActorWeeklyCleanEventHandler() {}

bool ActorWeeklyCleanEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_WEEKLY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorWeeklyCleanEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorWeeklyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorWeeklyCleanEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorWeeklyClean) || 
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorWeeklyClean *event = (event::EventActorWeeklyClean *)message;
  if (event->__isset.actor_ == true) {
    PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_);
    if (NULL == playing_actor) {
      LOG_ERROR("PlayingActor(%lu) not exists.", event->actor_);
      return;
    }
    playing_actor->PeriodClean(entity::PlayingPeriodType::WEEKLY);
  } else {
    // 清理所有玩家
    PlayingActorManager::GetInstance()->PeriodClean(entity::PlayingPeriodType::WEEKLY);
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

