//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 22:44:12.
// File name: storage_cooling_login_response.h
//
// Description:
// Define class StorageCoolingLoginResponse.
//

#ifndef __GAME__SERVER__COOLING__STORAGE__COOLING__LOGIN__RESPONSE__H
#define __GAME__SERVER__COOLING__STORAGE__COOLING__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace cooling {

class StorageCoolingLoginResponse : public core::Noncopyable {
 public:
  StorageCoolingLoginResponse();
  ~StorageCoolingLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__STORAGE__COOLING__LOGIN__RESPONSE__H

