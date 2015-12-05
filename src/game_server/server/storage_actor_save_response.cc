//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 02:26:50.
// File name: storage_actor_save_response.cc
//
// Description:
// Define class StorageActorSaveResponse.
//

#include "game_server/server/storage_actor_save_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_save_types.h"
#include "global/logging.h"
#include "global/global_packet.h"

namespace game {

namespace server {

StorageActorSaveResponse::StorageActorSaveResponse() {}
StorageActorSaveResponse::~StorageActorSaveResponse() {}

bool StorageActorSaveResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_SAVE, boost::bind(
        &StorageActorSaveResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageActorSaveResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_ACTOR_SAVE);
}

void StorageActorSaveResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageActorSaveRequest request;
  database::protocol::StorageActorSaveResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) 基本数据保存失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }
  if(response.result_ == false) {
    global::LogError("%s:%d (%s) 玩家(%ld) 基本数据保存失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }
}

}  // namespace server

}  // namespace game

