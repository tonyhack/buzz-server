//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-13 17:15:55.
// File name: storage_task_login_response.h
//
// Description:
// Define class StorageTaskLoginResponse.
//

#ifndef __GAME__SERVER__TASK__STORAGE__TASK__LOGIN__RESPONSE__H
#define __GAME__SERVER__TASK__STORAGE__TASK__LOGIN__RESPONSE__H

#include <string>

#include "core/base/noncopyable.h"
#include "database_server/protocol/database_protocol_types.h"

namespace game {

namespace server {

namespace task {

class StorageTaskLoginResponse : public core::Noncopyable {
 public:
  StorageTaskLoginResponse();
  ~StorageTaskLoginResponse();

  bool Initialize();
  void Finalize();

 private:
  void OnResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

  void OnActorResponse(const std::string &req, const std::string &res,
      database::protocol::ResponseType::type result);

};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__STORAGE__TASK__LOGIN__RESPONSE__H
