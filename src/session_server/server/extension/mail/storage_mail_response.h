//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-13 23:54:07.
// File name: storage_mail_response.h
//
// Description:
//
#ifndef __SESSION__SERVER__STORAGE__MAIL__RESPONSE__H
#define __SESSION__SERVER__STORAGE__MAIL__RESPONSE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace session {

namespace server {

namespace mail {

class StorageMailResponse {
 public:
  StorageMailResponse();
  ~StorageMailResponse();

  bool Initialize();
  void Finalize();
 private:
  void response(const std::string &req, const std::string &res, 
       database::protocol::ResponseType::type result);
};

} // namespace mail

} // namespace server

} // namespace session

#endif // __SESSION__SERVER__STORAGE__MAIL__RESPONSE__H

