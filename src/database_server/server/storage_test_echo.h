//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-18 14:18:20.
// File name: storage_test_echo.h
//
// Description:
// Define class StorageTestEcho.
//

#ifndef __DATEBASE__SERVER__STORAGE__TEST__ECHO__H
#define __DATEBASE__SERVER__STORAGE__TEST__ECHO__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageTestEcho : public core::Noncopyable {
 public:
  StorageTestEcho();
  ~StorageTestEcho();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageTestEchoResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATEBASE__SERVER__STORAGE__TEST__ECHO__H

