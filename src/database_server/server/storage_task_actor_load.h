//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-16 23:27:08.
// File name: storage_task_actor_load.h
//
// Description:
//
#ifndef __DATABASE__SERVER__STORAGE__TASK__ACTOR__LOAD__H
#define __DATABASE__SERVER__STORAGE__TASK__ACTOR__LOAD__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_task_actor_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageTaskActorLoad : public core::Noncopyable {
 public:
  StorageTaskActorLoad();
  ~StorageTaskActorLoad();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageTaskActorLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__TASK__ACTOR__LOAD__H

