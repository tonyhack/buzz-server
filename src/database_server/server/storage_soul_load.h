//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 11:08:26.
// File name: storage_soul_load.h
//
// Description:
// Define class StorageSoulLoad.
//

#ifndef __DATABASE__SERVER__STORAGE__SOUL__LOAD__H
#define __DATABASE__SERVER__STORAGE__SOUL__LOAD__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_soul_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageSoulLoad : public core::Noncopyable {
 public:
  StorageSoulLoad();
  ~StorageSoulLoad();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageSoulLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__SOUL__LOAD__H

