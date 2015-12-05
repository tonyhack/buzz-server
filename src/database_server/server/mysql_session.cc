//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 18:22:57.
// File name: mysql_session.cc
//
// Description:
// Define class MysqlSession.
//

#include "database_server/server/mysql_session.h"

#include "core/async_logging_singleton.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace database {

namespace server {

MysqlSession::MysqlSession()
  : connecting_(false), connection_(false), query_(&connection_, false) {}
MysqlSession::~MysqlSession() {}

bool MysqlSession::Initialize(const global::DatabaseInformation &database) {
  this->host_ = database.ip_;
  this->port_ = database.port_;
  this->user_ = database.user_;
  this->passwd_ = database.passwd_;
  this->db_ = database.db_;

  const global::GameDatabaseInformation *info =
    global::ServerConfigureSingleton::GetInstance().GetGameDatabaseInformation();
  if(info == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get game database information failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  int pos = protocol::StorageType::STORAGE_START;
  for(; pos < protocol::StorageType::STORAGE_MAX; ++pos) {
    global::GameDatabaseInformation::ProcedureMap::const_iterator iterator =
        info->procedures_.find(pos);
    if(iterator == info->procedures_.end()) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Procedure[%d] not exist in configure.",
          __FILE__, __LINE__, __FUNCTION__, pos);
      return false;
    }
    this->procedure_[pos] = iterator->second;
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_DEBUG,
        "Procedure %d=%s", pos, this->procedure_[pos].c_str());
  }

  return true;
}

bool MysqlSession::Start() {
  return this->Connecting();
}


void MysqlSession::Stop() {
  this->Disconnect();
}

bool MysqlSession::Reconnect() {
  if(this->CheckConnectStatus() == false) {
    if(this->connection_.connected() == true) {
      this->connection_.disconnect();
    }
    return this->Connecting();
  }
  return true;
}

void MysqlSession::ResetQuery() {
  this->query_.reset();
  while(this->query_.more_results()) {
    this->query_.store_next();
  }
}

bool MysqlSession::Connecting() {
  mysqlpp::MultiStatementsOption *option1 =
      new (std::nothrow) mysqlpp::MultiStatementsOption(CLIENT_MULTI_STATEMENTS);
  if(option1 == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate msyqlpp::MultiStatementsOption failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  mysqlpp::SetCharsetNameOption *option2 =
      new (std::nothrow) mysqlpp::SetCharsetNameOption("UTF8");
  if(option2 == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate msyqlpp::SetCharsetNameOption failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->connection_.set_option(option1);
  this->connection_.set_option(option2);

  if(this->connection_.connect(this->db_.c_str(), this->host_.c_str(),
        this->user_.c_str(), this->passwd_.c_str(), this->port_) == false) {
    this->connecting_ = false;
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Connect mysql[db=%s, host=%s:%d, user=%s, passwd=%s] error=[%s]",
        __FILE__, __LINE__, __FUNCTION__, this->db_.c_str(), this->host_.c_str(),
        this->port_, this->user_.c_str(), this->passwd_.c_str(), this->connection_.error());
    return false;
  } else {
    this->query_ = this->connection_.query();
    this->connecting_ = true;
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "%s:%d (%s) Connect mysql[db=%s, host=%s:%d] complete!",
        __FILE__, __LINE__, __FUNCTION__, this->db_.c_str(),
        this->host_.c_str(), this->port_);
    return true;
  }
}

}  // namespace server

}  // namespace database

