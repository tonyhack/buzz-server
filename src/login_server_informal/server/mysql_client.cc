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

#include "login_server_informal/server/mysql_client.h"

#include "core/async_logging_singleton.h"
#include "login_server_informal/server/server_configure.h"

namespace login {

namespace server {

MysqlClient::MysqlClient() {}
MysqlClient::~MysqlClient() {}

bool MysqlClient::Initialize() {
  // Get database configure.
  const GameDatabaseInformation *information =
    ServerConfigure::GetInstance()->GetDatabaseInfo();
  if(information == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get database information configure failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  // Allocate and initialize center database session.
  this->center_sessions_.reset(new (std::nothrow) MysqlSession());
  if(this->center_sessions_.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->center_sessions_->Initialize(information->center_database_) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
  }
  // Allocate and initialize game database sessions.
  if(information->databases_.empty()) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) No mysql information in configure file.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  GameDatabaseInformation::DatabaseMap::const_iterator iterator =
    information->databases_.begin();
  for(; iterator != information->databases_.end(); ++iterator) {
    MysqlSessionVector vec;
    GameDatabaseInformation::DatabaseVector::const_iterator iterator2 =
      iterator->second.begin();
    for(; iterator2 != iterator->second.end(); ++iterator2) {
      MysqlSessionPtr session(new (std::nothrow) MysqlSession());
      if(session.get() == NULL) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Allocate mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
      if(session->Initialize(*iterator2) == false) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Initialize mysql session failed.", __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
      vec.push_back(session);
    }
    this->zones_.insert(std::make_pair(iterator->first, vec));
  }
  return true;
}

bool MysqlClient::Start() {
  if(this->center_sessions_->Start() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Start center mysql session failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
  MysqlZoneMap::iterator iterator1 = this->zones_.begin();
  for(; iterator1 != this->zones_.end(); ++iterator1) {
    MysqlSessionVector::iterator iterator2 = iterator1->second.begin();
    for(; iterator2 != iterator1->second.end(); ++iterator2) {
      if((*iterator2)->Start() == false) {
        AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
            "%s:%d (%s) Start game mysql session failed.",
            __FILE__, __LINE__, __FUNCTION__);
        return false;
      }
    }
  }
  AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
      "%s:%d (%s) Start mysql sessions complete.",
      __FILE__, __LINE__, __FUNCTION__);
  return true;
}

void MysqlClient::Stop() {
  MysqlZoneMap::iterator iterator1 = this->zones_.begin();
  for(; iterator1 != this->zones_.end(); ++iterator1) {
    MysqlSessionVector::iterator iterator2 = iterator1->second.begin();
    for(; iterator2 != iterator1->second.end(); ++iterator2) {
      (*iterator2)->Stop();
    }
  }
  this->center_sessions_->Stop();
}

}  // namespace server

}  // namespace login

