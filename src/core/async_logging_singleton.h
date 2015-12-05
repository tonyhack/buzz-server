//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:34:06.
// File name: async_logging_singleton.h
//
// Description:
// Define class AsyncLoggingSingleton.
//

#ifndef __CORE__ASYNC__LOGGING__SINGLETON__H
#define __CORE__ASYNC__LOGGING__SINGLETON__H

#include "core/base/noncopyable.h"
#include "core/async_logging.h"

namespace core {

extern const int kMaxLoggingSize;

class AsyncLoggingSingleton : public core::Noncopyable {
 public:
  AsyncLoggingSingleton() {}
  ~AsyncLoggingSingleton() {}

  static inline AsyncLogging *GetAsyncLogging() {
    return &AsyncLoggingSingleton::async_logging_;
  }

 private:
  static AsyncLogging async_logging_;
};

}  // namespace core

typedef core::AsyncLoggingSingleton AsyncLoggingSingleton;

#endif  // __CORE__ASYNC__LOGGING__SINGLETON__H

