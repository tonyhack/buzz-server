#include "game_server/server/extension/attr/storage_spirit_facade_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_spirit_facade_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"

namespace game {

namespace server {

namespace attr {

StorageSpiritFacadeLoginResponse::StorageSpiritFacadeLoginResponse() {}
StorageSpiritFacadeLoginResponse::~StorageSpiritFacadeLoginResponse() {}

bool StorageSpiritFacadeLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_SPIRIT_FACADE_LOGIN,
      boost::bind(&StorageSpiritFacadeLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageSpiritFacadeLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_SPIRIT_FACADE_LOGIN);
}

void StorageSpiritFacadeLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageSpiritFacadeLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize message :StorageSpiritFacadeLoginRequest failed.");
    return;
  }
  database::protocol::StorageSpiritFacadeLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message StorageSpiritFacadeLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::SpiritFacadeLoginResult::SUCCESS) {
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
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", request.actor_id_);
    return;
  }

  // 初始化精灵
  if (attr_actor->InitializeSpiritActor(response.fields_) == false) {
    LOG_ERROR("Init AttrActor(%lu)'s SpiritActor failed.", request.actor_id_);
    return;
  }

  // 发送加载完成事件
  if (attr_actor->CheckLoadFinish()) {
    attr_actor->SendLoadFinishEvent();
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

