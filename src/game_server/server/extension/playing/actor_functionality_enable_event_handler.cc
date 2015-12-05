#include "game_server/server/extension/playing/actor_functionality_enable_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "entity/functionality_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/playing/playing_actor.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"

namespace game {

namespace server {

namespace playing {

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
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if (NULL == actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->actor_id_);
    return;
  }

  // 副本扩展是否已存在
  PlayingActor *playing_actor = PlayingActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == playing_actor) {
    LOG_ERROR("Get PlayingActor(%lu) from PlayingActorManager failed.", event->actor_id_);
    return;
  }

  if (entity::FunctionalityType::MAZE_MODULE == event->functionality_id_) {
    if (playing_actor->GetMazeActor()->EnableMazeFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable MAZE_MODULE failed.", event->actor_id_);
      return;
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace game

