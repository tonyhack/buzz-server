//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 12:16:29.
// File name: storage_item_login_response.h
//
// Description:
// Define class StorageItemLoginResponse.
//

#ifndef __GAME__SERVER__ITEM__STORAGE__ITEM__LOGIN__RESPONSE__H
#define __GAME__SERVER__ITEM__STORAGE__ITEM__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace item {

class StorageItemLoginResponse : public core::Noncopyable {
 public:
  StorageItemLoginResponse();
  ~StorageItemLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__STORAGE__ITEM__LOGIN__RESPONSE__H

