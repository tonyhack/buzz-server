//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 11:21:04.
// File name: storage_item_load.h
//
// Description:
// Define class StorageItemLoad.
//

#ifndef __DATABASE__SERVER__STORAGE__ITEM__LOAD__H
#define __DATABASE__SERVER__STORAGE__ITEM__LOAD__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_item_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageItemLoad : public core::Noncopyable {
 public:
  StorageItemLoad();
  ~StorageItemLoad();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageItemLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__ITEM__LOAD__H

