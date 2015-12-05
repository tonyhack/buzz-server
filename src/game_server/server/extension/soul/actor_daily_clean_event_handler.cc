#include "game_server/server/extension/soul/actor_daily_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"

namespace game {

namespace server {

namespace soul {

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

  if (event->__isset.actor_) {
    // 清理单个玩家
    SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->actor_);
    if (NULL == soul_actor) {
      LOG_ERROR("Get SoulActor(%lu) from SoulActorManager failed.", event->actor_);
      return;
    }
    soul_actor->DailyClean();
  } else {
    // 清理所有玩家
    SoulActorManager::GetInstance()->DailyClean();
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

