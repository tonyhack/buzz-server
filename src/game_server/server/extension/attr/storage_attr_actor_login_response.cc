#include "game_server/server/extension/attr/storage_attr_actor_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_attr_actor_login_types.h"
#include "database_server/protocol/storage_spirit_facade_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor_pool.h"

namespace game {

namespace server {

namespace attr {

StorageAttrActorLoginResponse::StorageAttrActorLoginResponse() {}
StorageAttrActorLoginResponse::~StorageAttrActorLoginResponse() {}

bool StorageAttrActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ATTR_ACTOR_LOGIN,
      boost::bind(&StorageAttrActorLoginResponse::OnResponse, this, _1, _2, _3));

  return true;
}

void StorageAttrActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ATTR_ACTOR_LOGIN);
}

void StorageAttrActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageAttrActorLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize message StorageAttrActorLoginRequest failed.");
    return;
  }
  database::protocol::StorageAttrActorLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message StorageAttrActorLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::AttrActorLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageAttrLoginRequest failed.");
    return;
  }

  // 从管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get GameActor(%lu) from GameActorManager failed.", request.actor_id_);
    return;
  }

  // 属性扩展是否已存在
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(request.actor_id_);
  if (attr_actor != NULL) {
    LOG_ERROR("AttrActor(%lu) already exists.", request.actor_id_);
    return;
  }

  // 分配属性玩家对象
  attr_actor = AttrActorPool::GetInstance()->Allocate();
  if (NULL == attr_actor) { 
    LOG_ERROR("Allocate AttrActor(%lu) from AttrActorPool failed.", request.actor_id_);
    return;
  }

  // 初始化
  if (attr_actor->Initialize(game_actor, response.field_) == false) {
    AttrActorPool::GetInstance()->Deallocate(attr_actor);
    LOG_ERROR("Init AttrActor(%lu) failed.", request.actor_id_);
    return;
  }

  // 加入管理器
  if (AttrActorManager::GetInstance()->Add(attr_actor) == false) {
    attr_actor->Finalize();
    AttrActorPool::GetInstance()->Deallocate(attr_actor);
    LOG_ERROR("Add AttrActor(%lu) to AttrActorManager failed.", request.actor_id_);
    return;
  }

  // 精灵外观数据库请求
  database::protocol::StorageSpiritFacadeLoginRequest spirit_facade_request;
  spirit_facade_request.__set_actor_id_(request.actor_id_);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        spirit_facade_request,
        database::protocol::StorageType::STORAGE_SPIRIT_FACADE_LOGIN,
        request.actor_id_) == false) {
    LOG_ERROR("Send StorageSpiritFacadeLoginRequest to database server failed.");
    return;
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

