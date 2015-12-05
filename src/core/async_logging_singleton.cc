//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:34:17.
// File name: async_logging_singleton.cc
//
// Description:
// Define class AsyncLoggingSingleton.
//


#include "core/async_logging_singleton.h"

namespace core {

const int kMaxLoggingSize = 10 * 1024;

AsyncLogging AsyncLoggingSingleton::async_logging_;

}  // namespace core

