//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-30 13:43:05.
// File name: storage_soul_actor_login_response.cc
//
// Description:
// Define class StorageSoulActorLoginResponse.
//

#include "game_server/server/extension/soul/storage_soul_actor_login_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_soul_login_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/soul/soul_actor.h"
#include "game_server/server/extension/soul/soul_actor_manager.h"
#include "game_server/server/extension/soul/soul_actor_pool.h"
#include "global/logging.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace soul {

StorageSoulActorLoginResponse::StorageSoulActorLoginResponse() {}
StorageSoulActorLoginResponse::~StorageSoulActorLoginResponse() {}

bool StorageSoulActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_SOUL_ACTOR_LOGIN, boost::bind(
        &StorageSoulActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageSoulActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_SOUL_ACTOR_LOGIN);
}

void StorageSoulActorLoginResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageSoulActorLoginRequest request;
  database::protocol::StorageSoulActorLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 英灵玩家登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, result);
    return ;
  }
  if(response.result_ != database::protocol::SoulActorLoginResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 英灵玩家登陆失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.actor_id_, response.result_);
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", request.actor_id_);
    return ;
  }

  SoulActor *soul_actor = SoulActorManager::GetInstance()->Get(request.actor_id_);
  if(soul_actor != NULL) {
    LOG_ERROR("SoulActor(%lu) 已存在", request.actor_id_);
    return ;
  }

  soul_actor = SoulActorPool::GetInstance()->Allocate();
  if(soul_actor == NULL) {
    LOG_ERROR("分配 SoulActor(%lu) 失败", request.actor_id_);
    return ;
  }

  if(soul_actor->Initialize(actor, response.field_) == false) {
    SoulActorPool::GetInstance()->Deallocate(soul_actor);
    LOG_ERROR("初始化 SoulActor(%lu) 失败", request.actor_id_);
    return ;
  }

  if(SoulActorManager::GetInstance()->Add(soul_actor) == false) {
    soul_actor->Finalize();
    SoulActorPool::GetInstance()->Deallocate(soul_actor);
    LOG_ERROR("SoulActor(%lu) 加入管理器失败", request.actor_id_);
    return ;
  }

  // 请求加载英灵数据
  database::protocol::StorageSoulLoginRequest request_login;
  request_login.__set_actor_id_(request.actor_id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(request_login,
        database::protocol::StorageType::STORAGE_SOUL_LOGIN,
        request.actor_id_) == false) {
    LOG_ERROR("玩家(%lu) 请求 StorageSoulLoginRequest 失败", request.actor_id_);
    return ;
  }
}

}  // namespace soul

}  // namespace server

}  // namespace game


