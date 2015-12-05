//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-17 10:32:22.
// File name: async_request_service.cc
//
// Description:
// Define class AsyncRequestService.
//

#include "database_server/server/async_request_service.h"

#include "core/async_logging_singleton.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"

namespace database {

namespace server {

AsyncRequestService::AsyncRequestService() {}
AsyncRequestService::~AsyncRequestService() {}

bool AsyncRequestService::Initialize(size_t queue_size, int queue_reduce_waterline,
size_t thread_number, coresh::CompressType::Type compress, coresh::EncodeType::Type encode) {
  if(this->pool_.Initialize(queue_size, queue_reduce_waterline,
        thread_number, compress, encode) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize async request pool failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  return true;
}

bool AsyncRequestService::Start() {
  if(this->pool_.Start() == false) {
    return false;
  }
  this->reconnector_.Start();
  return true;
}

void AsyncRequestService::Stop() {
  this->reconnector_.Stop();
  this->pool_.Stop();
}

bool AsyncRequestService::Request(const char *message,
    size_t size, core::uint64 channel) {
  return this->pool_.Request(message, size, channel);
}

}  // namespace server

}  // namespace database

