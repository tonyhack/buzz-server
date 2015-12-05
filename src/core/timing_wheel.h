//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:52.
// File name: timing_wheel.h
//
// Description:
// Define class TimingWheel.
//

#ifndef __CORE__TIMING__WHEEL__H
#define __CORE__TIMING__WHEEL__H

#include <vector>

#include "core/channel.h"
#include "core/timing_base.h"
#include "core/timing_group.h"
#include "core/version.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"

namespace core {

class Timer;
class TimerHandler;
class EventLoop;

class TimingWheel {
  typedef std::vector<Timer *> TimerReleaseList;

 public:
  TimingWheel();
  ~TimingWheel();

  bool Initialize(uint32 tick_msec, EventLoop *loop);
  void Finalize();

  // Start and set the timestamp.
  bool Start(const TimestampMillisecond &now);

  // Schedule for user.
  void Schedule(uint32 id, uint32 tick_count, int call_times,
         TimerHandler *handler, const std::string &debug);
  
  // Schedule for TimingWheel/TimingBase/TimingGroup.
  bool ScheduleImplement(Timer *timer, uint32 tick_count);
  
  // Unschedule for user.
  void Unschedule(uint32 id, TimerHandler *handler);
  
  // Callback.
  void OnSchedule(const TimeTick &now);
  
  // Forwrad to previous group.
  bool Forward(int group_id, Timer *timer);

  bool ResetTimer();

  // Release the memory
  void TimerRelease();

 private:
  TimingBase base_;
  TimingGroup group_[4];
  
  uint32 tick_msec_;
  TimestampMillisecond timestamp_;

  EventLoop *loop_;

#ifdef __CORE_KERNEL_TIMERFD__
  Channel channel_;
#else  // __CORE_KERNEL_TIMERFD__
  uint32 patch_msec_;
#endif  // __CORE_KERNEL_TIMERFD__

  TimerReleaseList releases_;
};

}  // namespace core

#endif  // __CORE__TIMING__WHEEL__H

