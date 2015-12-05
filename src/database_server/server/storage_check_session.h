//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-25 14:18:30.
// File name: storage_check_session.h
//
// Description:
// Define class StorageCheckSession.
//

#ifndef __DATABASE__SERVER__STORAGE__CHECK__SESSION__H
#define __DATABASE__SERVER__STORAGE__CHECK__SESSION__H

#include "core/base/noncopyable.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/storage_check_session_types.h"

namespace database {

namespace server {

class AsyncRequestLoop;

class StorageCheckSession : public core::Noncopyable {
 public:
  StorageCheckSession();
  ~StorageCheckSession();

  bool Initialize(AsyncRequestLoop *async_request_loop);

 private:
  void Request(const protocol::AsyncRequestMessage &message);

  void ResponseFailed(protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  void OnResponse(const protocol::StorageCheckSessionResponse &response,
      protocol::ResponseType::type result, const protocol::AsyncRequestMessage &request);

  AsyncRequestLoop *async_request_loop_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__STORAGE__CHECK__SESSION__H

