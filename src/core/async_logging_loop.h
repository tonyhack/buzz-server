//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:32:58.
// File name: async_logging_loop.h
//
// Description:
// Define class AsyncLoggingLoop.
//

#ifndef __CORE__ASYNC__LOGGING__LOOP__H
#define __CORE__ASYNC__LOGGING__LOOP__H

#include <boost/shared_ptr.hpp>

#include "core/base_loop.h"
#include "core/base/condition_variable.h"
#include "core/logging_level.h"

namespace core {

class ByteBufferDynamic;

class AsyncLoggingLoop : public core::BaseLoop {
  typedef boost::shared_ptr<core::ByteBufferDynamic> ByteBufferDynamicPtr;
 public:
  AsyncLoggingLoop();
  virtual ~AsyncLoggingLoop();

  bool Initialize(const std::string file, int max_logging_size, int flush_interval_msecs = 5);

  virtual void Loop();
  virtual void Wakeup();

  void Logging(LoggingLevel level, const char *log);

 private:
  std::string file_;
  std::string file_time_;
  std::string file_suffix_;

  std::string temp_file_;

  ByteBufferDynamicPtr logging_buffer_;
  core::Mutex mutex_;
  core::ConditionVariable condition_;

  LoggingLevel level_;

  int flush_interval_msecs_;

  char buffer_[10240];
};

}  // namespace core

#endif  // __CORE__ASYNC__LOGGING__LOOP__H

