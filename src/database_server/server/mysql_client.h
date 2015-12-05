//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-21 09:50:42.
// File name: mysql_client.h
//
// Description:
// Define class MysqlClient.
//

#ifndef __DATABASE__SERVER__MYSQL__CLIENT__H
#define __DATABASE__SERVER__MYSQL__CLIENT__H

#include <map>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/server/mysql_session.h"
#include "global/channel.h"

namespace database {

namespace server {

class MysqlClient : public core::Noncopyable {
 public:
  MysqlClient();
  ~MysqlClient();

  bool Initialize();
  bool Start();
  void Stop();

  inline MysqlSessionPtr GetSession(core::uint64 channel) {
    return this->game_session_;
  }

  inline MysqlSessionPtr GetCenterSession() {
    return this->center_session_;
  }

 private:
  MysqlSessionPtr center_session_;
  MysqlSessionPtr game_session_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__MYSQL__CLIENT__H

