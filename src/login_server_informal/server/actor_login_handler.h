//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-06 15:55:18.
// File name: actor_login_handler.h
//
// Description:
// Define class ActorLoginHandler.
//

#ifndef __LOGIN__SERVER__GET__ACTOR__H
#define __LOGIN__SERVER__GET__ACTOR__H

#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"
#include "login_server_informal/protocol/login_protocol_types.h"

namespace login {

namespace server {

class ActorLoginHandler : public core::Noncopyable {
 public:
  ActorLoginHandler();
  ~ActorLoginHandler();

  bool OnRequest(const core::TCPConnectionPtr &connection,
      const char *message, size_t size);

 private:
  void ResponseFailed(const core::TCPConnectionPtr &connection);

  void Response(const core::TCPConnectionPtr &connection,
      const protocol::MessageActorLoginResponse &response);
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__GET__ACTOR__H

