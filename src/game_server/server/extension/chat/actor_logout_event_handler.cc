#include "game_server/server/extension/chat/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/chat/chat_actor.h"
#include "game_server/server/extension/chat/chat_actor_manager.h"
#include "game_server/server/extension/chat/chat_actor_pool.h"

namespace game {

namespace server {

namespace chat {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登出事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorLogoutEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorLogout) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 移除聊天扩展
  ChatActor *chat_actor = ChatActorManager::GetInstance()->Remove(event->id_);
  if (NULL == chat_actor) {
    LOG_ERROR("Remove ChatActor(%lu) from ChatActorManager failed", event->id_);
    return;
  }
  chat_actor->Finalize();
  ChatActorPool::GetInstance()->Deallocate(chat_actor);
}

}  // namespace chat

}  // namespace server

}  // namespace game

