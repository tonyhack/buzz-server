#include "game_server/server/extension/chat/chat_actor.h"

#include <cstddef>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension_manager.h"

namespace game {

namespace server {

namespace chat {

ChatActor::ChatActor() : game_actor_(NULL) {}
ChatActor::~ChatActor() {}

bool ChatActor::Initialize(GameActor *game_actor) {
  if (NULL == game_actor) {
    LOG_ERROR("game_actor is null.");
    return false;
  }

  this->game_actor_ = game_actor;

  this->last_chat_time_.resize(entity::ChatChannelType::MAX, 0);

  return true;
}

void ChatActor::Finalize() {
  this->last_chat_time_.clear();
  this->game_actor_ = NULL;
}

void ChatActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::CHAT);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
  LOG_DEBUG("玩家(%lu) CHAT扩展加载成功", this->GetID());
}

time_t ChatActor::GetLastChatTime(entity::ChatChannelType::type channel) {
  if (channel < entity::ChatChannelType::MIN ||
      channel >= entity::ChatChannelType::MAX) {
    return (time_t)-1;
  }
  return this->last_chat_time_[channel];
}

void ChatActor::SetLastChatTime(entity::ChatChannelType::type channel, time_t chat_time) {
  if (channel < entity::ChatChannelType::MIN ||
      channel >= entity::ChatChannelType::MAX) {
    return;
  }
  this->last_chat_time_[channel] = chat_time;
}

}  // namespace chat

}  // namespace server

}  // namespace game

