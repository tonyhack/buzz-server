#include "game_server/server/extension/playing/actor_vip_change_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"
#include "game_server/server/extension/playing/maze_actor.h"

namespace game {

namespace server {

namespace playing {

ActorVipChangeEventHandler::ActorVipChangeEventHandler() {}
ActorVipChangeEventHandler::~ActorVipChangeEventHandler() {}

bool ActorVipChangeEventHandler::Initialize() {
  // Follow 所有玩家VIP等级变更事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_VIP_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorVipChangeEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorVipChangeEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorVipChangeEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorVipChange) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorVipChange *event = (event::EventActorVipChange *)message;

  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_);
  if (NULL == playing_actor) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", event->actor_);
    return;
  }
  playing_actor->GetMazeActor()->AdjustActionPoint(event->old_type_, event->new_type_);
}

}  // namespace playing

}  // namespace server

}  // namespace game

