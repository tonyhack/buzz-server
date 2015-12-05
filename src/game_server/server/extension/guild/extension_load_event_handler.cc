#include "game_server/server/extension/guild/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/guild/guild_actor.h"
#include "game_server/server/extension/guild/guild_actor_manager.h"

namespace game {

namespace server {

namespace guild {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow BUFF 模块加载完成事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ExtensionLoadEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ExtensionLoadEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorExtensionLoadFinish) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if ((ExtensionType::type)event->extension_type_ != ExtensionType::BUFF) {
    return;
  }

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(event->id_);
  if (NULL == guild_actor) {
    return;
  }

  guild_actor->InitializeGuildBuff();
  guild_actor->SendLoadFinishEvent();
}

}  // namespace guild

}  // namespace server

}  // namespace game

