#include "session_server/server/sync_database_operation_manager.h"

#include "global/logging.h"
#include "global/server_configure.h"
#include "global/server_informations.h"

namespace session {

namespace server {

void SyncDatabaseOperationManager::AddLoader(const DatabaseOperation &loader) {
  this->loaders_.push_back(loader);
}

void SyncDatabaseOperationManager::AddSaver(const DatabaseOperation &saver) {
  this->savers_.push_back(saver);
}

bool SyncDatabaseOperationManager::LoadFromDatabase() {
  return this->DatabaseQuery(this->loaders_);
}

bool SyncDatabaseOperationManager::SaveToDatabase() {
  return this->DatabaseQuery(this->savers_);
}

static bool InitializeDatabaseConnection(mysqlpp::Connection &connection) {
  // Get database configure.
  const global::GameDatabaseInformation *information =
    global::ServerConfigureSingleton::GetInstance().GetGameDatabaseInformation();
  if (NULL == information) {
    LOG_ERROR("Get database information configure failed.");
    return false;
  }

  // Setup database options.
  mysqlpp::MultiStatementsOption *option1 =
    new (std::nothrow) mysqlpp::MultiStatementsOption(CLIENT_MULTI_STATEMENTS);
  if (NULL == option1) {
    LOG_ERROR("Allocate msyqlpp::MultiStatementsOption failed.");
    return false;
  }   
  mysqlpp::SetCharsetNameOption *option2 =
    new (std::nothrow) mysqlpp::SetCharsetNameOption("UTF8");
  if (NULL == option2) {
    LOG_ERROR("Allocate msyqlpp::SetCharsetNameOption failed.");
    return false;
  }   
  connection.set_option(option1);
  connection.set_option(option2);

  // Connect database.
  if (connection.connect(information->game_database_.db_.c_str(), information->game_database_.ip_.c_str(),
        information->game_database_.user_.c_str(), information->game_database_.passwd_.c_str()) == false) {
    LOG_ERROR("Connect mysql[db=%s, host=%s:%d, user=%s, passwd=%s] error=[%s]",
        information->game_database_.db_.c_str(), information->game_database_.ip_.c_str(),
        information->game_database_.user_.c_str(), information->game_database_.passwd_.c_str(),
        connection.error());
    return false;
  }

  return true;
}

bool SyncDatabaseOperationManager::DatabaseQuery(const DatabaseOperation &operation) {
  mysqlpp::Connection connection(false);

  if (InitializeDatabaseConnection(connection) == false) {
    LOG_ERROR("Init database connection failed.");
    return false;
  }

  if (operation(connection) == false) {
    connection.disconnect();
    return false;
  }

  connection.disconnect();

  return true;
}

bool SyncDatabaseOperationManager::DatabaseQuery(const DatabaseOperationVector &operations) {

  mysqlpp::Connection connection(false);

  if (InitializeDatabaseConnection(connection) == false) {
    LOG_ERROR("Init database connection failed.");
    return false;
  }

  for (size_t i = 0; i < operations.size(); ++i) {
    if (operations[i](connection) == false) {
      connection.disconnect();
      return false;
    }
  }

  connection.disconnect();

  return true;
}

}  // namespace server

}  // namespace session

