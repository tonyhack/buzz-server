//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:32:38.
// File name: async_logging.h
//
// Description:
// Define class AsyncLogging.
//

#ifndef __CORE__ASYNC__LOGGING__H
#define __CORE__ASYNC__LOGGING__H

#include <boost/shared_ptr.hpp>

#include "core/loop_thread.h"
#include "core/base/noncopyable.h"
#include "core/async_logging_loop.h"
#include "core/logging_level.h"

namespace core {

class AsyncLogging : public core::Noncopyable {
  typedef boost::shared_ptr<core::LoopThread> LoopThreadPtr;
 public:
  AsyncLogging();
  ~AsyncLogging();

  bool Initialize(const std::string file, int max_logging_size);
  inline void SetLoggingLevel(LoggingLevel level) {
    this->logging_level_ = level;
  }

  void Logging(LoggingLevel level, const char *log, ...);
  void Logging2(LoggingLevel level, const char *log);

  void Start();
  void Stop();

 private:
  AsyncLoggingLoop loop_;
  LoopThreadPtr loop_thread_;

  LoggingLevel logging_level_;
};

}  // namespace core

#endif  // __CORE__ASYNC__LOGGING__H

