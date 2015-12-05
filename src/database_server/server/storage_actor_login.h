//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-19 16:34:20.
// File name: storage_actor_login.h
//
// Description:
// Define class StorageActorLogin.
//

#ifndef __DATABASE__SERVER__STORAGE__ACTOR__LOGIN__H
#define __DATABASE__SERVER__STORAGE__ACTOR__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_actor_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageActorLogin : public core::Noncopyable {
 public:
  StorageActorLogin();
  ~StorageActorLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageActorLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__ACTOR__LOGIN__H

