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

#ifndef __LOGIN__SERVER__MYSQL__CLIENT__H
#define __LOGIN__SERVER__MYSQL__CLIENT__H

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "login_server_informal/server/mysql_session.h"
#include "global/channel.h"

namespace login {

namespace server {

class MysqlClient : public core::Noncopyable {
  typedef std::vector<MysqlSessionPtr> MysqlSessionVector;
  typedef std::map<core::uint16, MysqlSessionVector> MysqlZoneMap;

 public:
  MysqlClient();
  ~MysqlClient();

  bool Initialize();
  bool Start();
  void Stop();

  inline MysqlSessionPtr GetSession(core::uint64 channel) {
    core::uint16 zone = global::Channel::GetZoneID(channel);
    MysqlZoneMap::iterator iterator = this->zones_.find(zone);
    if(iterator == this->zones_.end()) {
      return MysqlSessionPtr();
    }
    if(iterator->second.empty()) {
      return MysqlSessionPtr();
    }
    return iterator->second[channel % iterator->second.size()];
  }

  inline MysqlSessionPtr GetCenterSession() {
    return this->center_sessions_;
  }

 private:
  MysqlZoneMap zones_;
  MysqlSessionPtr center_sessions_;
};

typedef boost::shared_ptr<MysqlClient> MysqlClientPtr;

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__MYSQL__CLIENT__H

