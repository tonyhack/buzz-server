//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-30 16:25:24.
// File name: log_proxy.h
//
// Description:
// Define class LogProxy.
//

#ifndef __GAME__SERVER__PYTHON__LOG__PROXY__H
#define __GAME__SERVER__PYTHON__LOG__PROXY__H

#include <boost/shared_ptr.hpp>

#include "core/base/types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace python {

class LogProxy {
 public:
  LogProxy() {}
  ~LogProxy() {}

  void LogInfo(const char *log) {
    global::LogInfo(log);
  }

  void LogDebug(const char *log) {
    global::LogDebug(log);
  }

  void LogError(const char *log) {
    global::LogError(log);
  }

  void LogWarning(const char *log) {
    global::LogWarning(log);
  }
};

typedef boost::shared_ptr<LogProxy> LogProxyPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__LOG__PROXY__H

