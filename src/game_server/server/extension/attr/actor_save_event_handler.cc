#include "game_server/server/extension/attr/actor_save_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"

namespace game {

namespace server {

namespace attr {

ActorSaveEventHandler::ActorSaveEventHandler() {}
ActorSaveEventHandler::~ActorSaveEventHandler() {}

bool ActorSaveEventHandler::Initialize() {
  // Follow 所有玩家保存事件
  this->conn_actor_save_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_SAVE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorSaveEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorSaveEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_save_);
}

void ActorSaveEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorSave) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorSave *event = (event::EventActorSave *)message;

  LOG_DEBUG("玩家(%lu) ATTR扩展保存");

  AttrActor *actor = AttrActorManager::GetInstance()->Get(event->id_);
  if (NULL == actor) {
    LOG_ERROR("AttrActor(%lu) does not found in AttrActorManager.", event->id_);
    return;
  }

  actor->Save();
}

}  // namespace attr

}  // namespace server

}  // namespace game

