//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 18:16:04.
// File name: async_request_thread.h
//
// Description:
// Define class AsyncRequestThread.
//

#ifndef __DATABASE__SERVER__ASYNC__REQUEST__THREAD__H
#define __DATABASE__SERVER__ASYNC__REQUEST__THREAD__H

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "core/loop_thread.h"
#include "core/base/mutex.h"
#include "core/base/noncopyable.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "database_server/server/async_request_loop.h"
#include "global/thrift_packet.h"

namespace database {

namespace server {

class AsyncRequestThread : public core::Noncopyable,
  public boost::enable_shared_from_this<AsyncRequestThread> {
 public:
  AsyncRequestThread();
  ~AsyncRequestThread();

  bool Initialize(size_t queue_size, int queue_reduce_waterline,
      coresh::CompressType::Type compress, coresh::EncodeType::Type encode);

  bool Request(const char *message, size_t size);

  bool Start();
  void Stop();

 private:
  core::LoopThread thread_;
  AsyncRequestLoop loop_;

  global::ThriftPacket packet_;
  core::Mutex mutex_;
};

typedef boost::shared_ptr<AsyncRequestThread> AsyncRequestThreadPtr;

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__ASYNC__REQUEST__THREAD__H

