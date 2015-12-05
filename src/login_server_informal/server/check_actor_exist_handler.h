//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-07-08 10:37:30.
// File name: check_actor_exist_handler.h
//
// Description:
// Define class CheckActorExistHandler.
//

#ifndef __LOGIN__SERVER__CHECK__ACTOR__EXIST__HANDLER__H
#define __LOGIN__SERVER__CHECK__ACTOR__EXIST__HANDLER__H

#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"
#include "login_server_informal/protocol/login_protocol_types.h"

namespace login {

namespace server {

class CheckActorExistHandler : public core::Noncopyable {
 public:
  CheckActorExistHandler();
  ~CheckActorExistHandler();

  bool OnRequest(const core::TCPConnectionPtr &connection,
      const char *message, size_t size);

 private:
  void ResponseFailed(const core::TCPConnectionPtr &connection);

  void Response(const core::TCPConnectionPtr &connection,
      const protocol::MessageCheckActorExistResponse &response);
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__CHECK__ACTOR__EXIST__HANDLER__H

