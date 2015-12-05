//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 11:36:26.
// File name: extension_load_event_handler.cc
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#include "game_server/server/extension/soul/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_soul_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_actor_pool.h"
#include "global/channel.h"

namespace game {

namespace server {

namespace soul {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  this->conn_extension_load_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ExtensionLoadEventHandler::OnEventActorExtensionLoadFinish,
        this, _1, _2, _3, _4));

  this->conn_actor_load_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOAD_FINISH,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ExtensionLoadEventHandler::OnEventActorLoadFinish,
        this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_extension_load_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_load_);
}

void ExtensionLoadEventHandler::OnEventActorExtensionLoadFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if((ExtensionType::type)event->extension_type_ == ExtensionType::SKILL) {
    // 从玩家管理器中获取玩家对象
    GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
    if(actor == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 英灵扩展登陆失败，获取 GameActor 失败",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 英灵玩家对象是否存在
    SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->id_);
    if(soul_actor) {
      global::LogError("%s:%d (%s) 玩家(%lu) 英灵扩展已存在 SoulActorManager",
          __FILE__, __LINE__, __FUNCTION__, event->id_);
      return ;
    }

    // 数据库请求
    database::protocol::StorageSoulActorLoginRequest request;
    request.__set_actor_id_(event->id_);
    if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(request,
          database::protocol::StorageType::STORAGE_SOUL_ACTOR_LOGIN, event->id_) == false) {
      LOG_ERROR("玩家(%lu) 请求 StorageSoulActorLoginRequest 失败", event->id_);
      return ;
    }
  }
}

void ExtensionLoadEventHandler::OnEventActorLoadFinish(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLoadFinish *event = (event::EventActorLoadFinish *)message;

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(event->id_);
  if(soul_actor == NULL) {
    LOG_ERROR("获取 SoulActor(%lu) 失败", event->id_);
    return ;
  }

  // 开启英灵数据
  if (soul_actor->LoadFinish() == false) {
    SoulActorManager::GetInstance()->Remove(soul_actor->GetID());
    soul_actor->Finalize();
    SoulActorPool::GetInstance()->Deallocate(soul_actor);
    LOG_ERROR("SoulActor(%lu) LoadFinish 失败", event->id_);
    return;
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game

