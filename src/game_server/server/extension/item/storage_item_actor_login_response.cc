#include "game_server/server/extension/item/storage_item_actor_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_item_actor_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_pool.h"
#include "game_server/server/extension/item/item_actor_manager.h"

namespace game {

namespace server {

namespace item {

StorageItemActorLoginResponse::StorageItemActorLoginResponse() {}
StorageItemActorLoginResponse::~StorageItemActorLoginResponse() {}

bool StorageItemActorLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ITEM_ACTOR_LOGIN,
      boost::bind(&StorageItemActorLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageItemActorLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ITEM_ACTOR_LOGIN);
}

void StorageItemActorLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageItemActorLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize message StorageItemActorLoginRequest failed.");
    return;
  }
  database::protocol::StorageItemActorLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message StorageItemActorLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::ItemActorLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageItemActorLoginRequest failed.");
    return;
  }

  // 从管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(request.actor_id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get GameActor(%lu) from GameActorManager failed.", request.actor_id_);
    return;
  }

  // 道具扩展是否已存在
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(request.actor_id_);
  if (item_actor != NULL) {
    LOG_ERROR("ItemActor(%lu) already exists.", request.actor_id_);
    return;
  }

  // 分配属性玩家对象
  item_actor = ItemActorPool::GetInstance()->Allocate();
  if (NULL == item_actor) {
    LOG_ERROR("Allocate ItemActor(%lu) from ItemActorPool failed.", request.actor_id_);
    return;
  }

  // 初始化
  if (item_actor->Initialize(game_actor, response.field_) == false) {
    ItemActorPool::GetInstance()->Deallocate(item_actor);
    LOG_ERROR("Init ItemActor(%lu) failed.", request.actor_id_);
    return;
  }

  // 加入管理器
  if (ItemActorManager::GetInstance()->Add(item_actor) == false) {
    item_actor->Finalize();
    ItemActorPool::GetInstance()->Deallocate(item_actor);
    LOG_ERROR("Add ItemActor(%lu) to ItemActorManager failed.", request.actor_id_);
    return;
  }

  // 背包栏道具数据库请求
  database::protocol::StorageItemLoginRequest item_request;
  item_request.__set_actor_id_(request.actor_id_);
  item_request.__set_contain_type_(entity::ItemContainerType::PACKET);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        item_request, database::protocol::StorageType::STORAGE_ITEM_LOGIN, request.actor_id_) == false) {
    LOG_ERROR("Send StorageItemLoginRequest(packet) to database server failed.");
    return;
  }

  // 装备栏道具数据库请求
  item_request.__set_actor_id_(request.actor_id_);
  item_request.__set_contain_type_(entity::ItemContainerType::EQUIP);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        item_request, database::protocol::StorageType::STORAGE_ITEM_LOGIN, request.actor_id_) == false) {
    LOG_ERROR("Send StorageItemLoginRequest(equip) to database server failed.");
    return;
  }

  // 礼品仓库道具请求
  item_request.__set_actor_id_(request.actor_id_);
  item_request.__set_contain_type_(entity::ItemContainerType::TREASURE);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        item_request, database::protocol::StorageType::STORAGE_ITEM_LOGIN, request.actor_id_) == false) {
    LOG_ERROR("Send StorageItemLoginRequest(equip) to database server failed.");
    return;
  }

  // 仓库栏道具数据库请求
  item_request.__set_actor_id_(request.actor_id_);
  item_request.__set_contain_type_(entity::ItemContainerType::STORAGE);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        item_request, database::protocol::StorageType::STORAGE_ITEM_LOGIN, request.actor_id_) == false) {
    LOG_ERROR("Send StorageItemLoginRequest(storage) to database server failed.");
    return;
  }

  // 宝石数据库请求
  database::protocol::StorageGemLoginRequest gem_request;
  gem_request.__set_actor_id_(request.actor_id_);
  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        gem_request, database::protocol::StorageType::STORAGE_GEM_LOGIN, request.actor_id_) == false) {
    LOG_ERROR("Send StorageGemLoginRequest to database server failed.");
    return;
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

