#ifndef __SESSION__SERVER__SYNC__DATABASE__OPERATION__MANAGER__H
#define __SESSION__SERVER__SYNC__DATABASE__OPERATION__MANAGER__H

#include <boost/function.hpp>
#include <mysql++/mysql++.h> 

#include "global/singleton_factory.h"

namespace session {

namespace server {

class SyncDatabaseOperationManager : public global::SingletonFactory<SyncDatabaseOperationManager> { 
  friend class global::SingletonFactory<SyncDatabaseOperationManager>;

 public:
  typedef boost::function<bool (mysqlpp::Connection &)> DatabaseOperation;
  typedef std::vector<DatabaseOperation> DatabaseOperationVector;

  void AddLoader(const DatabaseOperation &loader);
  void AddSaver(const DatabaseOperation &saver);

  bool LoadFromDatabase();
  bool SaveToDatabase();
  bool DatabaseQuery(const DatabaseOperation &operation);

 private:
  SyncDatabaseOperationManager() {}
  ~SyncDatabaseOperationManager() {}

  bool DatabaseQuery(const DatabaseOperationVector &operations);

  DatabaseOperationVector loaders_;
  DatabaseOperationVector savers_;
};

}  // namespace server 

}  // namespace session

#endif  // __SESSION__SERVER__SYNC__DATABASE__OPERATION__MANAGER__H

