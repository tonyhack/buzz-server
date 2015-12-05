//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:36.
// File name: timing_group.h
//
// Description:
// Define class TimingGroup.
//

#ifndef __CORE__TIMING__GROUP__H
#define __CORE__TIMING__GROUP__H

#include "core/timer.h"

namespace core {

class TimingWheel;

class TimingGroup {
 public:
  explicit TimingGroup();
  ~TimingGroup();

  inline uint32 GetCurrentTick() const { return this->current_tick_; }

  inline void Initialize(TimingWheel *wheel, int id) {
    this->wheel_ = wheel;
    this->id_ = id;
  }

  inline void Schedule(Timer *timer, int tick_count) {
  /*
    if(this->id_ == 0) {
      timer->timing_bucket_id_ =
             (((((tick_count - timer->left_base_tick_count_) >> TIMING_BASE_BIT) +
             this->current_tick_) % TIMING_BASE_SIZE) - 1) % TIMING_GROUP_SIZE;
    } else {
      timer->timing_bucket_id_ =
        (((((tick_count - timer->left_base_tick_count_) >> (TIMING_BASE_BIT + this->id_ * TIMING_GROUP_BIT)) +
           this->current_tick_) % TIMING_GROUP_SIZE) - 1) % TIMING_GROUP_SIZE;
    }
  */

    timer->timing_bucket_id_ =
      (((((tick_count - timer->left_base_tick_count_) >> (TIMING_BASE_BIT + this->id_ * TIMING_GROUP_BIT)) +
         this->current_tick_) % TIMING_GROUP_SIZE) - 1) % TIMING_GROUP_SIZE;

    timer->iterator_ = bucket_[timer->timing_bucket_id_].insert(
           bucket_[timer->timing_bucket_id_].end(), timer);
  }

  inline Timer::TimerList &GetBucket(uint32 id) {
    return this->bucket_[id];
  }

  bool OnSchedule();

 private:
  int id_;
  uint32 current_tick_;
  Timer::TimerList bucket_[TIMING_GROUP_SIZE];
  TimingWheel *wheel_;
};

}  // namespace core

#endif  // __CORE__TIMING__GROUP__H

