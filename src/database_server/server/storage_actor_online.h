//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-04 14:47:40.
// File name: storage_actor_online.h
//
// Description:
// Define class StorageActorOnline.
//

#ifndef __DATABASE__SERVER__STORAGE__ACTOR__ONLINE__H
#define __DATABASE__SERVER__STORAGE__ACTOR__ONLINE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_actor_online_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageActorOnline : public core::Noncopyable {
 public:
  StorageActorOnline();
  ~StorageActorOnline();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageActorOnlineResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__ACTOR__ONLINE__H

