//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-18 11:33:41.
// File name: storage_soul_save.h
//
// Description:
// Define class StorageSoulSave.
//

#ifndef __DATABASE__SERVER__STORAGE__SOUL__SAVE__H
#define __DATABASE__SERVER__STORAGE__SOUL__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_soul_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageSoulSave : public core::Noncopyable {
 public:
  StorageSoulSave();
  ~StorageSoulSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageSoulSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__SOUL__SAVE__H

