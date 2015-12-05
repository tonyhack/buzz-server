//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 12:20:27.
// File name: storage_soul_login_response.h
//
// Description:
// Define class StorageSoulLoginResponse.
//

#ifndef __GAME__SERVER__SOUL__STORAGE__SOUL__LOGIN__RESPONSE__H
#define __GAME__SERVER__SOUL__STORAGE__SOUL__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace soul {

class StorageSoulLoginResponse : public core::Noncopyable {
 public:
  StorageSoulLoginResponse();
  ~StorageSoulLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__STORAGE__SOUL__LOGIN__RESPONSE__H

