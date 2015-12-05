//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 13:08:08.
// File name: storage_playing_actor_save.h
//
// Description:
// StoragePlayingActorSave.
//

#ifndef __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__SAVE__H
#define __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_playing_actor_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StoragePlayingActorSave : public core::Noncopyable {
 public:
  StoragePlayingActorSave();
  ~StoragePlayingActorSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StoragePlayingActorSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // database

#endif  // __DATABASE__SERVER__STORAGE__PLAYING__ACTOR__SAVE__H

