//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-17 00:02:54.
// File name: storage_task_actor_save.h
//
// Description:
//
#ifndef __DATABASE__SERVER__STORAGE__TASK__ACTOR__SAVE__H
#define __DATABASE__SERVER__STORAGE__TASK__ACTOR__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_task_actor_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageTaskActorSave : public core::Noncopyable {
 public:
  StorageTaskActorSave();
  ~StorageTaskActorSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageTaskActorSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__TASK__ACTOR__SAVE__H

