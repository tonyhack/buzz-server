//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 18:23:59.
// File name: mysql_session_group.h
//
// Description:
// Define class MysqlSessionGroup.
//

#ifndef __DATABASE__SERVER__MYSQL__SESSION__GROUP__H
#define __DATABASE__SERVER__MYSQL__SESSION__GROUP__H

#include <vector>

#include "database_server/database/mysql_session.h"

namespace database {

namespace server {

class MysqlSessionGroup : public core::Noncopyable {
  typedef std::vector<MysqlSessionPtr> MysqlSessionVector;

 public:
  MysqlSessionGroup();
  ~MysqlSessionGroup();
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__MYSQL__SESSION__GROUP__H

