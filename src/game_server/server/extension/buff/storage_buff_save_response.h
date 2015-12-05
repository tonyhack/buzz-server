//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-01 23:01:03.
// File name: storage_buff_save_response.h
//
// Description:
// Define class StorageBuffSaveResponse.
//

#ifndef __GAME__SERVER__BUFF__STORAGE__BUFF__SAVE__RESPONSE__H
#define __GAME__SERVER__BUFF__STORAGE__BUFF__SAVE__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace buff {

class StorageBuffSaveResponse : public core::Noncopyable {
 public:
  StorageBuffSaveResponse();
  ~StorageBuffSaveResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__STORAGE__BUFF__SAVE__RESPONSE__H

