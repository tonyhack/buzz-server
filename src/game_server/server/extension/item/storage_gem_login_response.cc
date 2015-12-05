#include "game_server/server/extension/item/storage_gem_login_response.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_gem_login_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"

namespace game {

namespace server {

namespace item {

StorageGemLoginResponse::StorageGemLoginResponse() {}
StorageGemLoginResponse::~StorageGemLoginResponse() {}

bool StorageGemLoginResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_GEM_LOGIN,
      boost::bind(&StorageGemLoginResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageGemLoginResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_GEM_LOGIN);
}

void StorageGemLoginResponse::OnResponse(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageGemLoginRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, req) == false) {
    LOG_ERROR("Deserialize message StorageGemLoginRequest failed.");
    return;
  }
  database::protocol::StorageGemLoginResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    LOG_ERROR("Deserialize message StorageGemLoginResponse failed.");
    return;
  }

  // 检测请求是否成功
  if (result != database::protocol::ResponseType::SUCCESS ||
      response.result_ != database::protocol::GemLoginResult::SUCCESS) {
    LOG_ERROR("Call StorageGemLoginRequest failed.");
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
  if (NULL == item_actor) {
    LOG_ERROR("Get ItemActor(%lu) from ItemActorManager failed.", request.actor_id_);
    return;
  }

  // 初始化宝石数据
  if (item_actor->InitializeGem(response.fields_) == false) {
    LOG_ERROR("Init ItemActor(%lu)'s gem data failed.", request.actor_id_);
    return;
  }

  if (item_actor->CheckLoadFinish()) {
    item_actor->SendLoadFinishEvent();
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

