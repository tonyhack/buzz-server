//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:20.
// File name: timing_base.h
//
// Description:
// Define class TimingBase.
//

#ifndef __CORE__TIMING__BASE__H
#define __CORE__TIMING__BASE__H

#include "core/timer.h"

namespace core {

class TimingWheel;

class TimingBase {
 public:
  explicit TimingBase();
  ~TimingBase();

  inline uint32 GetCurrentTick() const { return this->current_tick_; }

  inline void Initialize(TimingWheel *wheel) {
    this->wheel_ = wheel;
  }

  inline void Schedule(Timer *timer, uint32 tick_count) {
    timer->timing_bucket_id_ = (tick_count + this->current_tick_) % TIMING_BASE_SIZE;
    timer->iterator_ = this->bucket_[timer->timing_bucket_id_].insert(
           this->bucket_[timer->timing_bucket_id_].end(), timer);
  }


  inline Timer::TimerList &GetBucket(uint32 id) {
    return this->bucket_[id];
  }

  bool OnSchedule();

 private:
  uint32 current_tick_;
  Timer::TimerList bucket_[TIMING_BASE_SIZE];
  TimingWheel *wheel_;
};

}  // namespace core

#endif  // __CORE__TIMING__BASE__H

