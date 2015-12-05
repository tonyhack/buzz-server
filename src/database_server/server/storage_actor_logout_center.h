//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 17:18:59.
// File name: storage_actor_logout_center.h
//
// Description:
// Define class StorageActorLogoutCenter.
//

#ifndef __DATABASE__SERVER__STORAGE__ACTOR__LOGOUT__CENTER__H
#define __DATABASE__SERVER__STORAGE__ACTOR__LOGOUT__CENTER__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_actor_logout_center_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageActorLogoutCenter : public core::Noncopyable {
 public:
  StorageActorLogoutCenter();
  ~StorageActorLogoutCenter();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageActorLogoutCenterResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__ACTOR__LOGOUT__CENTER__H

