//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 19:25:43.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "game_server/server/extension/cooling/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_cooling_login_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/cooling/cooling_role.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace cooling {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_LOGIN,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR, 
        boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 冷却扩展登陆失败，获取 GameActor失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 冷却玩家对象是否已存在管理器中
  CoolingRole *role = CoolingRoleManager::GetInstance()->GetActor(event->id_);
  if(role) {
    global::LogError("%s:%d (%s) 玩家(%lu) 冷却扩展已存在 CoolingRoleManager",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 数据库请求
  database::protocol::StorageCoolingLoginRequest request;
  request.__set_id_(event->id_);
  size_t request_size = 0;
  const char *req = global::GlobalPacket::GetPacket()->Serialize(request, request_size);
  if(req == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 请求
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        database::protocol::StorageType::STORAGE_COOLING_LOGIN,
        event->id_, req, request_size) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

}  // namespace cooling

}  // namespace server

}  // namespace game

