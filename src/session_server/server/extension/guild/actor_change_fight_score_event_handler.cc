#include "session_server/server/extension/guild/actor_change_fight_score_event_handler.h"

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

ActorChangeFightScoreEventHandler::ActorChangeFightScoreEventHandler() {}
ActorChangeFightScoreEventHandler::~ActorChangeFightScoreEventHandler() {}

bool ActorChangeFightScoreEventHandler::Initialize() {
  // Follow 玩家战斗力变化事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_CHANGE_FIGHT_SCORE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorChangeFightScoreEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorChangeFightScoreEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorChangeFightScoreEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorChangeFightScore) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorChangeFightScore *event = (event::EventActorChangeFightScore *)message;
  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_id_);
    return;
  }
  guild_actor->SendFightScoreSync();
}

}  // namespace guild

}  // namespace server

}  // namespace session

