#ifndef __DATABASE__SERVER__STORAGE__GEM__SAVE__H
#define __DATABASE__SERVER__STORAGE__GEM__SAVE__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_gem_save_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageGemSave : public core::Noncopyable {
 public:
  StorageGemSave();
  ~StorageGemSave();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result, 
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageGemSaveResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__GEM__SAVE__H

