//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:35:49.
// File name: configure.h
//
// Description:
// Define class Configure.
//

#ifndef __CORE__CONFIG__H
#define __CORE__CONFIG__H

#include <pthread.h>

#include <string>

#include "core/base/mutex.h"
#include "core/base/noncopyable.h"
#include "core/tinyxml/tinyxml.h"

namespace core {

struct TCPConnectionConfig {
  int buffer_unit_size_;
  int buffer_init_size_;
  int bound_queue_size_;
};

struct ConnectorConfig {
  int retry_msec_;
};

struct TimerConfig {
  int unit_interval_msec_;
};

class Configure : public Noncopyable {
 public:
  Configure() {}
  ~Configure() {}

  static inline bool CheckFinishLoad() { return Configure::loaded_; }

  static void Initialize(const std::string &file) {
    Configure::mutex_.Lock();
    Configure::file_ = file;
    Configure::mutex_.Unlock();

    ::pthread_once(&Configure::once_, &Configure::LoadConfig);
  }

  static inline const TCPConnectionConfig *GetTCPConnection() {
    return &Configure::tcp_connection_;
  }
  static inline const ConnectorConfig *GetConnector() {
    return &Configure::connector_;
  }
  static inline const TimerConfig *GetTimer() {
    return &Configure::timer_;
  }

 private:
  static void LoadConfig();

  static std::string file_;
  static TCPConnectionConfig tcp_connection_;
  static ConnectorConfig connector_;
  static TimerConfig timer_;

  static Mutex mutex_;

  static pthread_once_t once_;

  static bool loaded_;
};

}  // namespace core

#endif  // __CORE__CONFIG__H

