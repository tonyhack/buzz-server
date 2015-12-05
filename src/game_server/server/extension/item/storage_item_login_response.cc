//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 13:40:14.
// File name: storage_item_login_response.cc
//
// Description:
// Define class StorageItemLoginResponse.
//

#include "game_server/server/extension/item/storage_item_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_item_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"

namespace game {

namespace server {

namespace item {

StorageItemLoginResponse::StorageItemLoginResponse() {}
StorageItemLoginResponse::~StorageItemLoginResponse() {}

bool StorageItemLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ITEM_LOGIN, boost::bind(
        &StorageItemLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageItemLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ITEM_LOGIN);
}

void StorageItemLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageItemLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize message StorageItemLoginRequest failed.");
    return;
  }
  database::protocol::StorageItemLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message StorageItemLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::ItemLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageItemLoginRequest failed.");
    return;
  }

  // 从管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get GameActor(%lu) from GameActorManager failed.", request.actor_id_);
    return;
  }

  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(request.actor_id_);
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", request.actor_id_);
    return;
  }

  // 初始化道具数据
  if (item_actor->InitializeItem((entity::ItemContainerType::type)request.contain_type_,
        response.fields_) == false) {
    LOG_ERROR("Init ItemActor(%lu)'s item data(type=[%d]) failed.",
              request.actor_id_, request.contain_type_);
    return;
  }

  if (item_actor->CheckLoadFinish() == true) {
    item_actor->SendLoadFinishEvent();
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

