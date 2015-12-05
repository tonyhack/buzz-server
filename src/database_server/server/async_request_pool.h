//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 18:18:31.
// File name: async_request_pool.h
//
// Description:
// Define class AsyncRequestPool.
//

#ifndef __DATABASE__SERVER__ASYNC__REQUEST__POOL__H
#define __DATABASE__SERVER__ASYNC__REQUEST__POOL__H

#include <vector>

#include <boost/shared_ptr.hpp>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"

namespace database {

namespace server {

class AsyncRequestThread;

typedef boost::shared_ptr<AsyncRequestThread> AsyncRequestThreadPtr;

class AsyncRequestPool : public core::Noncopyable {
  typedef std::vector<AsyncRequestThreadPtr> ThreadVector;

 public:
  AsyncRequestPool();
  ~AsyncRequestPool();

  bool Initialize(size_t queue_size, int queue_reduce_waterline, size_t thread_number,
      coresh::CompressType::Type compress, coresh::EncodeType::Type encode);

  bool Request(const char *message, size_t size, core::uint64 channel);

  bool Start();
  void Stop();

 private:
  ThreadVector threads_;
};

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__ASYNC__REQUEST__POOL__H

