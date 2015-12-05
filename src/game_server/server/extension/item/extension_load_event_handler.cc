//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 15:20:27.
// File name: extension_load_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/item/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_item_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace item {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow SKILL 模块加载完成事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ExtensionLoadEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
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

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (NULL == actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->id_);
    return;
  }

  // 道具玩家对象是否存在于管理器中
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(event->id_);
  if (item_actor != NULL) {
    LOG_ERROR("ItemActor(%lu) already exists.", event->id_);
    return;
  }

  // 向数据库请求数据
  database::protocol::StorageItemActorLoginRequest request;
  request.__set_actor_id_(event->id_);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_ITEM_ACTOR_LOGIN, event->id_) == false) {
    LOG_ERROR("Send StorageItemActorLoginRequest to database server failed.");
    return;
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

