//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-01 23:08:34.
// File name: storage_buff_save_response.cc
//
// Description:
// Define class StorageBuffSaveResponse.
//

#include "game_server/server/extension/buff/storage_buff_save_response.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_buff_save_types.h"
#include "global/logging.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace buff {

StorageBuffSaveResponse::StorageBuffSaveResponse() {}
StorageBuffSaveResponse::~StorageBuffSaveResponse() {}

bool StorageBuffSaveResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_BUFF_SAVE, boost::bind(
        &StorageBuffSaveResponse::OnResponse, this, _1, _2, _3));
  return true;
}

void StorageBuffSaveResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_BUFF_SAVE);
}

void StorageBuffSaveResponse::OnResponse(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageBuffSaveRequest request;
  database::protocol::StorageBuffSaveResponse response;

  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%ld) buff扩展保存失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }
  if(response.result_ == false) {
    global::LogError("%s:%d (%s) 玩家(%ld) buff扩展保存失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  global::LogDebug("玩家(%lu) buff扩展保存成功");
}

}  // namespace buff

}  // namespace server

}  // namespace game

