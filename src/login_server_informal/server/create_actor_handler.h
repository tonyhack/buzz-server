//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 16:20:35.
// File name: create_actor_handler.h
//
// Description:
// Define class CreateActorHandler.
//

#ifndef __LOGIN__SERVER__CREATE__ACTOR__HANDLER__H
#define __LOGIN__SERVER__CREATE__ACTOR__HANDLER__H

#include "core/tcp_connection.h"
#include "core/base/noncopyable.h"
#include "login_server_informal/protocol/login_protocol_types.h"
#include "login_server_informal/server/mysql_client.h"

namespace login {

namespace server {

class CreateActorHandler : public core::Noncopyable {
 public:
  CreateActorHandler();
  ~CreateActorHandler();

  bool OnRequest(const core::TCPConnectionPtr &connection,
      const char *message, size_t size);

 private:
  bool CreateGameActor(const MysqlClientPtr &mysql_client,
      core::uint64 id, const std::string name, core::int32 hair,
      bool male, core::int32 vocation);

  void ResponseFailed(const core::TCPConnectionPtr &connection,
      protocol::CreateActorType::type type);

  void Response(const core::TCPConnectionPtr &connection,
      const protocol::MessageCreateActorResponse &response);
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__CREATE__ACTOR__HANDLER__H

