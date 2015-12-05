#ifndef __DATABASE__SERVER__STORAGE__GUILD__ACTOR__LOGIN__H
#define __DATABASE__SERVER__STORAGE__GUILD__ACTOR__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_guild_actor_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageGuildActorLogin : public core::Noncopyable {
 public:
  StorageGuildActorLogin();
  ~StorageGuildActorLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result, 
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageGuildActorLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__GUILD__ACTOR__LOGIN__H

