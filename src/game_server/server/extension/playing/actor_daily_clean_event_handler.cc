//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-29 14:52:47.
// File name: actor_daily_clean_event_handler.cc
//
// Description:
// Define class ActorDailyCleanEventHandler.
//

#include "game_server/server/extension/playing/actor_daily_clean_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace playing {

ActorDailyCleanEventHandler::ActorDailyCleanEventHandler() {}
ActorDailyCleanEventHandler::~ActorDailyCleanEventHandler() {}

bool ActorDailyCleanEventHandler::Initialize() {
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
    PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_);
    if(playing_actor == NULL) {
      LOG_ERROR("PlayingActor(%lu) not exists.", event->actor_);
      return ;
    }
    playing_actor->PeriodClean(entity::PlayingPeriodType::DAILY);
  } else {
    // 清理所有玩家
    PlayingActorManager::GetInstance()->PeriodClean(entity::PlayingPeriodType::DAILY);
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

