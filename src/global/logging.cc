//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-28 13:52:12.
// File name: logging.cc
//
// Description:
// Define Logging.
//

#include "global/logging.h"

namespace global {

static const core::uint32 kLoggingMaxSize = 9 * 1024;

void LogError(const char *log, ...) {
  char buffer[kLoggingMaxSize + 1] = {0};
  GET_VARIABLE_PARAMETERS(buffer, sizeof(buffer), log);
  AsyncLoggingSingleton::GetAsyncLogging()->Logging2(core::LOGGING_LEVEL_ERROR,
      buffer);
}

void LogInfo(const char *log, ...) {
  char buffer[kLoggingMaxSize + 1] = {0};
  GET_VARIABLE_PARAMETERS(buffer, sizeof(buffer), log);
  AsyncLoggingSingleton::GetAsyncLogging()->Logging2(core::LOGGING_LEVEL_INFO,
      buffer);
}

void LogDebug(const char *log, ...) {
  char buffer[kLoggingMaxSize + 1] = {0};
  GET_VARIABLE_PARAMETERS(buffer, sizeof(buffer), log);
  AsyncLoggingSingleton::GetAsyncLogging()->Logging2(core::LOGGING_LEVEL_DEBUG,
      buffer);
}

void LogWarning(const char *log, ...) {
  char buffer[kLoggingMaxSize + 1] = {0};
  GET_VARIABLE_PARAMETERS(buffer, sizeof(buffer), log);
  AsyncLoggingSingleton::GetAsyncLogging()->Logging2(core::LOGGING_LEVEL_WARNING,
      buffer);
}

void LogFatal(const char *log, ...) {
  char buffer[kLoggingMaxSize + 1] = {0};
  GET_VARIABLE_PARAMETERS(buffer, sizeof(buffer), log);
  AsyncLoggingSingleton::GetAsyncLogging()->Logging2(core::LOGGING_LEVEL_FATAL,
      buffer);
}

}  // namespace global

