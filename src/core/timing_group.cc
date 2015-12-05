//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:42.
// File name: timing_group.cc
//
// Description:
// Define class TimingGroup.
//

#include "core/timing_group.h"

#include "core/timing_wheel.h"
#include "core/base/logging.h"

namespace core {

TimingGroup::TimingGroup() : current_tick_(0), wheel_(NULL) {}
TimingGroup::~TimingGroup() {}

bool TimingGroup::OnSchedule() {
  uint32 current_tick = this->current_tick_;
  if(++this->current_tick_ >= TIMING_GROUP_SIZE) {
    this->current_tick_ = 0;
  }
  Timer::TimerList::iterator iterator = this->bucket_[current_tick].begin();
  for(; iterator != this->bucket_[current_tick].end();) {
    Timer *timer = *iterator;
    // Clear from bucket.
    iterator = this->bucket_[current_tick].erase(iterator);
    timer->iterator_ = this->bucket_[current_tick].end();
    // Other timer maybe delete this timer.
    if(timer->releasing_)
      continue;
    if(timer->left_base_tick_count_ > 0) {
      // Tick count left, so forward to previous one.
      this->wheel_->Forward(this->id_, timer);
    } else {
      // No tick count left, so do callback.
      if(timer->OnSchedule() > 0) {
        // If need reschedule, first to check timer's valid(OnSchedule maybe Unschedule timer)
        if(timer->releasing_ == false) {
          this->wheel_->ScheduleImplement(timer, timer->tick_count_);
        }
      } else {
        this->wheel_->Unschedule(timer->id_, timer->handler_);
      }
    }
  }
  if(this->current_tick_ == 0) {
    return true;
  }
  return false;
}

}  // namespace core


