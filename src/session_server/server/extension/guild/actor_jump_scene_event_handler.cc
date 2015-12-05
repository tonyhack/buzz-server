#include "session_server/server/extension/guild/actor_jump_scene_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

ActorJumpSceneEventHandler::ActorJumpSceneEventHandler() {}
ActorJumpSceneEventHandler::~ActorJumpSceneEventHandler() {}

bool ActorJumpSceneEventHandler::Initialize() {
  // Follow 玩家跳场景事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_JUMP_SCENE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorJumpSceneEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorJumpSceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorJumpSceneEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorJumpScene) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorJumpScene *event = (event::EventActorJumpScene *)message;
  // 获取公会玩家对象
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->actor_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", event->actor_);
    return;
  }
  guild_actor->SendMapSync();
}

}  // namespace guild

}  // namespace server

}  // namespace session

