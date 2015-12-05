//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-17 09:36:14.
// File name: async_request_pool.cc
//
// Description:
// Define class AsyncRequestPool.
//

#include "database_server/server/async_request_pool.h"

#include "core/async_logging_singleton.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "database_server/server/async_request_thread.h"

namespace database {

namespace server {

AsyncRequestPool::AsyncRequestPool() {}
AsyncRequestPool::~AsyncRequestPool() {}

bool AsyncRequestPool::Initialize(size_t queue_size, int queue_reduce_waterline,
    size_t thread_number, coresh::CompressType::Type compress, coresh::EncodeType::Type encode) {
  size_t pos = 0;
  for(; pos < thread_number; ++pos) {
    AsyncRequestThreadPtr thread(new (std::nothrow) AsyncRequestThread());
    if(thread.get() == NULL) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Allocate async request thread failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    if(thread->Initialize(queue_size, queue_reduce_waterline, compress, encode) == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Initialize async request thread failed.",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }
    this->threads_.push_back(thread);
  }
  return true;
}

bool AsyncRequestPool::Request(const char *message,
    size_t size, core::uint64 channel) {
  return this->threads_[channel % this->threads_.size()]->Request(message, size);
}

bool AsyncRequestPool::Start() {
  ThreadVector::iterator iterator = this->threads_.begin();
  for(; iterator != this->threads_.end(); ++iterator) {
    if((*iterator)->Start() == false) {
      return false;
    }
  }
  return true;
}

void AsyncRequestPool::Stop() {
  ThreadVector::iterator iterator = this->threads_.begin();
  for(; iterator != this->threads_.end(); ++iterator) {
    (*iterator)->Stop();
  }
}

}  // namespace server

}  // namespace database

