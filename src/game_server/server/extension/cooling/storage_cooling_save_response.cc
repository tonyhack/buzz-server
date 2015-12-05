//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-01 23:08:34.
// File name: storage_cooling_save_response.cc
//
// Description:
// Define class StorageCoolingSaveResponse.
//

#include "game_server/server/extension/cooling/storage_cooling_save_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_cooling_save_types.h"
#include "global/logging.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace cooling {

StorageCoolingSaveResponse::StorageCoolingSaveResponse() {}
StorageCoolingSaveResponse::~StorageCoolingSaveResponse() {}

bool StorageCoolingSaveResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_COOLING_SAVE, boost::bind(
        &StorageCoolingSaveResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageCoolingSaveResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_COOLING_SAVE);
}

void StorageCoolingSaveResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageCoolingSaveRequest request;
  database::protocol::StorageCoolingSaveResponse response;

  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) 冷却扩展保存失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }
  if(response.result_ == false) {
    global::LogError("%s:%d (%s) 玩家(%ld) 冷却扩展保存失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }
}

}  // namespace cooling

}  // namespace server

}  // namespace game

