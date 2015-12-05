//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 10:01:53.
// File name: mysql_client.cc
//
// Description:
// Define class MysqlClient.
//

#include "database_server/server/mysql_client.h"

#include "core/async_logging_singleton.h"
#include "global/server_configure.h"

namespace database {

namespace server {

MysqlClient::MysqlClient() {}
MysqlClient::~MysqlClient() {}

bool MysqlClient::Initialize() {
  // Get database configure.
  const global::GameDatabaseInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameDatabaseInformation();
  if(information == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get database information configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Allocate and initialize center database session.
  this->center_session_.reset(new (std::nothrow) MysqlSession());
  if(this->center_session_.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->center_session_->Initialize(information->center_database_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Allocate and initialize game database session.
  this->game_session_.reset(new (std::nothrow) MysqlSession());
  if(this->game_session_.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->game_session_->Initialize(information->game_database_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

bool MysqlClient::Start() {
  if(this->center_session_->Start() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Start center mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->game_session_->Start() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Start game mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "%s:%d (%s) Start mysql sessions complete.",
      __FILE__, __LINE__, __FUNCTION__);
  return true;
}

void MysqlClient::Stop() {
  this->game_session_->Stop();
  this->center_session_->Stop();
}

}  // namespace server

}  // namespace database

