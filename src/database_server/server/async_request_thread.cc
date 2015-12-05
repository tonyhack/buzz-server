//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 18:16:47.
// File name: async_request_thread.cc
//
// Description:
// Define class AsyncRequestThread.
//

#include "database_server/server/async_request_thread.h"

#include "core/async_logging_singleton.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"

namespace database {

namespace server {

AsyncRequestThread::AsyncRequestThread() : thread_(&loop_, "AsyncRequestThread") {}
AsyncRequestThread::~AsyncRequestThread() {}

bool AsyncRequestThread::Initialize(size_t queue_size, int queue_reduce_waterline,
    coresh::CompressType::Type compress, coresh::EncodeType::Type encode) {
  if(this->loop_.Initialize(queue_size, queue_reduce_waterline, compress, encode) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize async request loop failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

bool AsyncRequestThread::Request(const char *message, size_t size) {
  return this->loop_.Request(message, size);
}

bool AsyncRequestThread::Start() {
  if(this->loop_.Start()) {
    this->thread_.StartLoop();
    return true;
  }
  return false;
}

void AsyncRequestThread::Stop() {
  this->thread_.StopLoop(true);
}

}  // namespace server

}  // namespace database

