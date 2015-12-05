//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 10:50:38.
// File name: mysql_reconnector.h
//
// Description:
// Define class MysqlReconnector.
//

#ifndef __DATABASE__SERVER__MYSQL__RECONNECTOR__H
#define __DATABASE__SERVER__MYSQL__RECONNECTOR__H

#include "database_server/server/mysql_session.h"

#include "global/reconnector.h"

namespace database {

namespace server {

class MysqlReconnectRequest {
 public:
  MysqlReconnectRequest() : valid_(true) {}
  MysqlReconnectRequest(const MysqlSessionPtr &session)
    : valid_(true), session_(session) {}

  ~MysqlReconnectRequest() {}

  inline bool CheckValid() const {
    return this->valid_;
  }

  inline void SetValid(bool valid) {
    this->valid_ = valid;
  }

  inline bool Reconnect() const {
    if(this->session_) {
      return this->session_->Reconnect();
    }
    return true;
  }

 private:
  bool valid_;
  MysqlSessionPtr session_;
};

typedef global::Reconnector<MysqlReconnectRequest> MysqlReconnector;

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__MYSQL__RECONNECTOR__H

