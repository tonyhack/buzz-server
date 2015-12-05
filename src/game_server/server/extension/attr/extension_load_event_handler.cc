#include "game_server/server/extension/attr/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"

namespace game {

namespace server {

namespace attr {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow BUFF 模块加载完成事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ExtensionLoadEventHandler::OnEvent,
        this, _1, _2, _3, _4));

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

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (NULL == actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->id_);
    return;
  }

  // 属性扩展是否已存在
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->id_);
  if (attr_actor != NULL) {
    LOG_ERROR("AttrActor(%lu) already exists.", event->id_);
    return;
  }

  // 向数据库请求数据
  database::protocol::StorageAttrActorLoginRequest request;
  request.__set_actor_id_(event->id_);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_ATTR_ACTOR_LOGIN,
          event->id_) == false) {
    LOG_ERROR("Send StorageAttrActorLoginRequest to database server failed.");
    return;
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

