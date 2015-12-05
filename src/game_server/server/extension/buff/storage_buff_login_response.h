//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 22:44:12.
// File name: storage_buff_login_response.h
//
// Description:
// Define class StorageBuffLoginResponse.
//

#ifndef __GAME__SERVER__BUFF__STORAGE__BUFF__LOGIN__RESPONSE__H
#define __GAME__SERVER__BUFF__STORAGE__BUFF__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace buff {

class StorageBuffLoginResponse : public core::Noncopyable {
 public:
  StorageBuffLoginResponse();
  ~StorageBuffLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__STORAGE__BUFF__LOGIN__RESPONSE__H

