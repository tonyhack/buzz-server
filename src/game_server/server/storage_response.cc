//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-27 10:51:47.
// File name: storage_response.cc
//
// Description:
// Define class StorageResponse.
//

#include "game_server/server/storage_response.h"

#include "global/logging.h"

namespace game {

namespace server {

StorageResponse::StorageResponse() {}
StorageResponse::~StorageResponse() {}

bool StorageResponse::Initialize() {
  if(this->actor_login_.Initialize() == false) {
    LOG_ERROR("Initialize actor login response failed.");
    return false;
  }
  if(this->actor_save_.Initialize() == false) {
    global::LogError("%s:%d (%s) Initialize actor save response failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

void StorageResponse::Finalize() {
  this->actor_save_.Finalize();
  this->actor_login_.Finalize();
}

}  // namespace server

}  // namespace game

