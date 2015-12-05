//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-01 15:14:59.
// File name: time_consuming.h
//
// Description: 
// Define class TimeConsuming.
//

#ifndef __GLOBAL__TIME__CONSUMING__H
#define __GLOBAL__TIME__CONSUMING__H

#include <string>

#include "core/base/time_tick.h"
#include "core/async_logging_singleton.h"

namespace global {

class TimeConsuming {
 public:
  TimeConsuming() {
    this->timestamp_.Stamp();
  }
  TimeConsuming(const std::string &debug) {
    this->timestamp_.Stamp();
    this->debug_ = debug;
  }
  explicit TimeConsuming(const TimeConsuming &time) {
    this->timestamp_ = time.timestamp_;
    this->debug_ = time.debug_;
  }

  ~TimeConsuming() {
    core::TimestampNanosecond now;
    now.Stamp();
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "[TimeConsuming] ~TimeConsuming debug[%s] now[%ld] cost [%ld] ns.",
        this->debug_.c_str(), now.GetStamp(), now.AbsoluteDistance(this->timestamp_));
  }

  inline void ConsumeLogging() {
    core::TimestampNanosecond now;
    now.Stamp();
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_INFO,
        "[TimeConsuming] ConsumeLogging debug[%s] now[%ld] cost [%ld] ns.",
        this->debug_.c_str(), now.GetStamp(), now.AbsoluteDistance(this->timestamp_));
  }

  inline void ResetConsuming() {
    this->timestamp_.Stamp();
  }
  inline void SetDebugInfo(const std::string &debug) {
    this->debug_ = debug;
  }
  inline void SetTimestamp(const core::TimestampNanosecond &timestamp) {
    this->timestamp_ = timestamp;
  }
  inline core::TimestampNanosecond GetTimestamp() const {
    return this->timestamp_;
  }

 private:
  core::TimestampNanosecond timestamp_;
  std::string debug_;
};

}  // namespace global

#endif  // __GLOBAL__TIME__CONSUMING__H

