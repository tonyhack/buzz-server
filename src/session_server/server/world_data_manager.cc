#include "session_server/server/world_data_manager.h"

#include <cstdlib>
#include <boost/bind.hpp>

#include "global/logging.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_world_data_save_types.h"
#include "session_server/server/sync_database_operation_manager.h"

namespace session {

namespace server {

bool WorldDataManager::Initialize() {
  if (SyncDatabaseOperationManager::GetInstance()->DatabaseQuery(
          boost::bind(&WorldDataManager::LoadWorldData, this, _1) == false)) {
    LOG_ERROR("Load world data failed.");
    return false;
  }

  return true;
}

void WorldDataManager::Finalize() {
}

std::string WorldDataManager::GetWorldData(WorldDataType::type id) {
  WorldDataHashmap::const_iterator iter = this->data_map_.find(id);
  if (iter != this->data_map_.end()) {
    return iter->second;
  } else {
    return std::string();
  }
}

void WorldDataManager::SaveWorldData(WorldDataType::type id, const std::string &data) {
  this->data_map_[id] = data;

  database::protocol::StorageWorldDataSaveRequest request;
  request.field_.__set_id_(id);
  request.field_.__set_data_(data);

  if (database::client::DatabaseClientSingleton::GetInstance().SendRequest(
          request, database::protocol::StorageType::STORAGE_WORLD_DATA_SAVE, 0) == false) {
    LOG_ERROR("Send StorageWorldDataSaveRequest to database server failed.");
    return;
  }
}

void WorldDataManager::SyncSaveWorldData(WorldDataType::type id, const std::string &data) {
  this->data_map_[id] = data;

  if (SyncDatabaseOperationManager::GetInstance()->DatabaseQuery(
          boost::bind(&WorldDataManager::SaveWorldData, this, _1, id, data)) == false) {
    LOG_ERROR("Sync save world data(%d) failed.", id);
    return;
  }
}

bool WorldDataManager::LoadWorldData(mysqlpp::Connection &connection) {
  mysqlpp::Query query(&connection, false);

  query << "CALL GAME_WORLD_DATA_LOAD()";
  mysqlpp::StoreQueryResult result = query.store();

  if (query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return false;
  }
  while (query.more_results()) {
    query.store_next();
  }
  query.reset();

  const char *fields[] = {
    "id",
    "data"
  };

  for (size_t i = 0; i < sizeof(fields) / sizeof(char *); ++i) {
    if ((size_t)result.field_num(fields[i]) != i) { 
      LOG_ERROR("Database field order is incorrect.");
      return false;
    }
  }

  for (size_t row = 0; row < result.num_rows(); ++row) {
    int idx = 0;
    core::int32 id = atoi(result[row][idx++].data());
    std::string data = result[row][idx++].data();

    this->data_map_[id] = data;
  }

  return true;
}

bool WorldDataManager::SaveWorldData(mysqlpp::Connection &connection,
    WorldDataType::type id, const std::string &data) {
  mysqlpp::Query query(&connection, false);

  query << "CALL GAME_WORLD_DATA_SAVE("
         << id << ", "
         << "'" << data << "'"
         << ")";
  mysqlpp::StoreQueryResult result = query.store();

  if (query.errnum() > 0) {
    LOG_ERROR("Procedure[%s] execute failed, error=[%s].",
              query.str().c_str(), query.error());
    return false;
  }
  while (query.more_results()) {
    query.store_next();
  }
  query.reset();

  return true;
}

}  // namespace server

}  // namespace session

