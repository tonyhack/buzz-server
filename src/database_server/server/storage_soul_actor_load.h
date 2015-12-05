//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-30 10:06:47.
// File name: storage_soul_actor_load.h
//
// Description:
// Define class StorageSoulActorLoad.
//

#ifndef __DATABASE__SERVER__STORAGE__SOUL__ACTOR__LOAD__H
#define __DATABASE__SERVER__STORAGE__SOUL__ACTOR__LOAD__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_soul_actor_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageSoulActorLoad : public core::Noncopyable {
 public:
  StorageSoulActorLoad();
  ~StorageSoulActorLoad();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageSoulActorLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__SOUL__ACTOR__LOAD__H

