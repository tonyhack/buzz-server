#include "session_server/server/extension/social/actor_save_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ActorSaveEventHandler::ActorSaveEventHandler() {}
ActorSaveEventHandler::~ActorSaveEventHandler() {}

bool ActorSaveEventHandler::Initialize() {
  // Follow 所有玩家保存事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_SAVE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorSaveEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorSaveEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorSaveEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorSave) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorSave *event = (event::EventActorSave *)message;

  SocialActor *social_actor = SocialActorManager::GetInstance()->Get(event->id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", event->id_);
    return;
  }

  social_actor->Save();
}

}  // namespace social

}  // namespace server

}  // namespace session

