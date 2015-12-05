//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 10:00:15.
// File name: storage_playing_actor_login.h
//
// Description:
// Define class StoragePlayingActorLogin.
//

#ifndef __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__LOGIN__H
#define __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_playing_actor_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StoragePlayingActorLogin : public core::Noncopyable {
 public:
  StoragePlayingActorLogin();
  ~StoragePlayingActorLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StoragePlayingActorLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__LOGIN__H

