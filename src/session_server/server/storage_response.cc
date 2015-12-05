//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 11:38:35.
// File name: storage_response.cc
//
// Description:
// Define class StorageResponse.
//

#include "session_server/server/storage_response.h"

#include "global/logging.h"

namespace session {

namespace server {

StorageResponse::StorageResponse() {}
StorageResponse::~StorageResponse() {}

bool StorageResponse::Initialize() {
  if(this->actor_login_.Initialize() == false) {
    LOG_ERROR("初始化玩家登陆回调失败");
    return false;
  }

  return true;
}

void StorageResponse::Finalize() {
  this->actor_login_.Finalize();
}

}  // namespace server

}  // namespace session

