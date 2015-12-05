#ifndef __DATABASE__SERVER__STORAGE__SPIRIT__FACADE__LOGIN__H
#define __DATABASE__SERVER__STORAGE__SPIRIT__FACADE__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_spirit_facade_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageSpiritFacadeLogin : public core::Noncopyable {
 public:
  StorageSpiritFacadeLogin();
  ~StorageSpiritFacadeLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result, 
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageSpiritFacadeLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__SPIRIT__FACADE__LOGIN__H

