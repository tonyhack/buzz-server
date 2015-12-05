//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 12:23:43.
// File name: storage_soul_login_response.cc
//
// Description:
// Define class StorageSoulLoginResponse.
//

#include "game_server/server/extension/soul/storage_soul_login_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_soul_login_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_actor_pool.h"
#include "global/logging.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace soul {

StorageSoulLoginResponse::StorageSoulLoginResponse() {}
StorageSoulLoginResponse::~StorageSoulLoginResponse() {}

bool StorageSoulLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_SOUL_LOGIN, boost::bind(
        &StorageSoulLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageSoulLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_SOUL_LOGIN);
}

void StorageSoulLoginResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageSoulLoginRequest request;
  database::protocol::StorageSoulLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 英灵扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, result);
    return ;
  }
  if(response.result_ != database::protocol::SoulLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 英灵扩展登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, response.result_);
    return ;
  }

  // 英灵玩家对象是否存在
  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(request.actor_id_);
  if(soul_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 获取英灵扩展失败",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_);
    return ;
  }

  // 导入英灵数据
  if(soul_actor->ImportSouls(response.fields_) == false) {
    SoulActorManager::GetInstance()->Remove(soul_actor->GetID());
    soul_actor->Finalize();
    SoulActorPool::GetInstance()->Deallocate(soul_actor);
    LOG_ERROR("SoulActor(%lu) 导入英灵数据失败", request.actor_id_);
    return ;
  }

  // 扩展加载完成事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.actor_id_);
  load_finish.__set_extension_type_(ExtensionType::SOUL);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.actor_id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu) 英灵扩展加载成功", soul_actor->GetID());
}

}  // namespace soul

}  // namespace server

}  // namespace game

