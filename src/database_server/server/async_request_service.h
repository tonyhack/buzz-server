//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-17 10:26:36.
// File name: async_request_service.h
//
// Description:
// Define class AsyncRequestService.
//

#ifndef __DATABASE__SERVER__ASYNC__REQUEST__SERVICE__H
#define __DATABASE__SERVER__ASYNC__REQUEST__SERVICE__H

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "database_server/server/async_request_pool.h"
#include "database_server/server/mysql_reconnector.h"

namespace database {

namespace server {

class AsyncRequestService : public core::Noncopyable {
 public:
  AsyncRequestService();
  ~AsyncRequestService();

  bool Initialize(size_t queue_size, int queue_reduce_waterline, size_t thread_number,
      coresh::CompressType::Type compress, coresh::EncodeType::Type encode);

  bool Start();
  void Stop();

  bool Request(const char *message, size_t size, core::uint64 channel);

  void Reconnect(const MysqlSessionPtr &session) {
    MysqlReconnectRequest request(session);
    this->reconnector_.Reconnect(request);
  }

 private:
  AsyncRequestPool pool_;
  MysqlReconnector reconnector_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__ASYNC__REQUEST__SERVICE__H

