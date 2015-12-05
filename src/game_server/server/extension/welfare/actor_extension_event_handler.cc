//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-05 17:47:08.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorExtensionEventHandler.
//
#include "game_server/server/extension/welfare/actor_extension_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"

namespace game {

namespace server {

namespace welfare {

ActorExtensionEventHandler::ActorExtensionEventHandler() {}
ActorExtensionEventHandler::~ActorExtensionEventHandler() {}

bool ActorExtensionEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_extension_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorExtensionEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorExtensionEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_extension_);
}

void ActorExtensionEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  if(event->extension_type_ != ExtensionType::TASK) {
    return ;
  }
  
  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展登陆失败，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  WelfareActor *welfare_actor = WelfareActorManager::GetInstance()->Get(event->id_);
  if(welfare_actor != NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展已存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 向数据库请求任务数据
  database::protocol::StorageWelfareLoadRequest request;
  request.__set_actor_(event->id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_WELFARE_LOAD,
        event->id_) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

}  // namespace welfare

}  // namespace server

}  // namespace game

