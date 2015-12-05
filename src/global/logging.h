//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-28 10:18:39.
// File name: logging.h
//
// Description:
// Define Logging.
//

#ifndef __GLOBAL__LOGGING__H
#define __GLOBAL__LOGGING__H

#include "core/async_logging_singleton.h"
#include "core/base/types.h"
#include "core/base/variable_parameter.h"

namespace global {

void LogError(const char *log, ...);

void LogInfo(const char *log, ...);

void LogDebug(const char *log, ...);

void LogWarning(const char *log, ...);

void LogFatal(const char *log, ...);


#define LOG_ERROR(C, ...) global::LogError("%s:%d (%s)" C, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define LOG_INFO(C, ...) global::LogInfo(C, ##__VA_ARGS__)

#define LOG_DEBUG(C, ...) global::LogDebug(C, ##__VA_ARGS__)

#define LOG_WARNING(C, ...) global::LogWarning("%s:%d (%s)" C, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define LOG_FATAL(C, ...) global::LogFatal("%s:%d (%s)" C, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

}  // namespace global

#endif  // __GLOBAL__LOGGING__H

