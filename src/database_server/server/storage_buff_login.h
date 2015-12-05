//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 20:11:42.
// File name: storage_buff_login.h
//
// Description:
// Define class StorageBuffLogin.
//

#ifndef __DATABASE__SERVER__STORAGE__BUFF__LOGIN__H
#define __DATABASE__SERVER__STORAGE__BUFF__LOGIN__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_buff_login_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageBuffLogin : public core::Noncopyable {
 public:
  StorageBuffLogin();
  ~StorageBuffLogin();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageBuffLoginResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__BUFF__LOGIN__H

