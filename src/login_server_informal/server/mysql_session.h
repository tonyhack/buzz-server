//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 18:21:41.
// File name: mysql_session.h
//
// Description:
// Define class MysqlSession.
//

#ifndef __LOGIN__SERVER__MYSQL__SESSION__H
#define __LOGIN__SERVER__MYSQL__SESSION__H

#include <string>

#include <boost/shared_ptr.hpp>
#include <mysql++/mysql++.h>

#include "core/base/types.h"
#include "core/base/noncopyable.h"
#include "login_server_informal/server/server_configure.h"
#include "login_server_informal/protocol/database_storage_types.h"

namespace login {

namespace server {

class MysqlSession : public core::Noncopyable {
 public:
  MysqlSession();
  ~MysqlSession();

  bool Initialize(const DatabaseInformation &database);

  bool Start();
  void Stop();
  bool Reconnect();

  inline bool CheckConnectStatus() const {
    return this->connecting_;
  }

  inline bool CheckConnected() {
    return this->connection_.connected();
  }
  inline bool PingConnected() {
    return this->connection_.connected() &&
      this->connection_.ping();
  }

  // Querier.
  inline mysqlpp::Query *GetQuery() {
    return &this->query_;
  }
  // Do store.
  inline const mysqlpp::StoreQueryResult *Store() {
    this->result_ = this->query_.store();
    return &this->result_;
  }

  inline void Disconnect() {
    this->connection_.disconnect();
    this->connecting_ = false;
  }

  const inline std::string &GetProcedure(protocol::StorageType::type type) const {
    return this->procedure_[type];
  }

  // Reset 
  void ResetQuery();

 private:
  bool Connecting();

  bool connecting_;

  std::string procedure_[protocol::StorageType::STORAGE_MAX];
  std::string db_;
  std::string host_;
  core::uint16 port_;
  std::string user_;
  std::string passwd_;

  mysqlpp::Connection connection_;
  mysqlpp::Query query_;
  mysqlpp::StoreQueryResult result_;
};

typedef boost::shared_ptr<MysqlSession> MysqlSessionPtr;

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__MYSQL__SESSION__H

