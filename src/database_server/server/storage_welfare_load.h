#ifndef __DATABASE__SERVER__STORAGE__WELFARE__LOAD__H
#define __DATABASE__SERVER__STORAGE__WELFARE__LOAD__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_welfare_load_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageWelfareLoad : public core::Noncopyable {
 public:
  StorageWelfareLoad();
  ~StorageWelfareLoad();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageWelfareLoadResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__WELFARE__LOAD__H

