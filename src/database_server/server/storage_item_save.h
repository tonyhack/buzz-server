//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-24 13:11:21.
// File name: storage_item_save.h
//
// Description:
// Define class StorageItemSave.
//

#ifndef __DATABASE__SERVER__STORAGE__ITEM__SAVE__H
#define __DATABASE__SERVER__STORAGE__ITEM__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_item_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageItemSave : public core::Noncopyable {
 public:
  StorageItemSave();
  ~StorageItemSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageItemSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__ITEM__SAVE__H

