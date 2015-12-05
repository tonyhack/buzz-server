//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:59.
// File name: timing_wheel.cc
//
// Description:
// Define class TimingWheel.
//

#include "core/timing_wheel.h"

#include <assert.h>
#ifdef __CORE_KERNEL_TIMERFD__
#include <sys/timerfd.h>
#endif  // __CORE_KERNEL_TIMERFD__

#include <boost/bind.hpp>

#include "core/event_loop.h"
#include "core/timer.h"
#include "core/base/logging.h"

namespace core {

TimingWheel::TimingWheel()
       : tick_msec_(50),
         loop_(NULL)
#ifndef __CORE_KERNEL_TIMERFD__
         , patch_msec_(0) {}
#else  // __CORE_KERNEL_TIMERFD__
         {}
#endif  // __CORE_KERNEL_TIMERFD__

TimingWheel::~TimingWheel() {}

bool TimingWheel::Initialize(uint32 tick_msec, EventLoop *loop) {
  if(loop == NULL) {
    CoreLog(ERROR, "Invalid param loop",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  this->loop_ = loop;
  this->tick_msec_ = tick_msec;

#ifdef __CORE_KERNEL_TIMERFD__

  int timer_fd = ::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
  if(timer_fd == -1) {
    CoreLog(ERROR, "%s:%d (%s) timerfd_create error.",
           __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->channel_.Initialize(this->loop_, timer_fd) == false) {
    CoreLog(ERROR, "%s:%d (%s) Initialize Channel failed.",
        __FILE__, __LINE__, __FUNCTION__);
    ::close(timer_fd);
    return false;
  }
  this->channel_.SetReadCallback(boost::bind(&TimingWheel::OnSchedule, this, _1));
  this->channel_.EnableRead();

#else  // __CORE_KERNEL_TIMERFD__

  this->loop_->TimeCallback(boost::bind(&TimingWheel::OnSchedule, this, _1));

#endif  // __CORE_KERNEL_TIMERFD__

  return true;
}

void TimingWheel::Finalize() {
  this->TimerRelease();
#ifdef __CORE_KERNEL_TIMERFD__

  this->channel_.DisableAll();
  ::close(this->channel_.GetFd());
  this->channel_.Finalize();

#endif  // __CORE_KERNEL_TIMERFD__
}

bool TimingWheel::Start(const TimestampMillisecond &now) {
  this->timestamp_ = now;

  this->base_.Initialize(this);
  this->group_[0].Initialize(this, 0);
  this->group_[1].Initialize(this, 1);
  this->group_[2].Initialize(this, 2);
  this->group_[3].Initialize(this, 3);

  return this->ResetTimer();
}

void TimingWheel::Schedule(uint32 id, uint32 tick_count, int call_times, TimerHandler *handler, const std::string &debug) {
  if(handler->timer_list_.find(id) != handler->timer_list_.end()) {
    // Reschedule to delete previous one.
    this->Unschedule(id, handler);
  }
  Timer *timer = new (std::nothrow) Timer(id, tick_count, call_times, handler, debug);
  if(timer == NULL) {
    return ;
  }
  if(this->ScheduleImplement(timer, tick_count) == false) {
    delete timer;
  }
#ifdef _CORE_DEBUG
  CoreLog(INFO, "TimingWheel::Schedule hander=[%llx] id=[%d] debug_info=[%s]", timer->handler_, timer->id_, timer->debug_.c_str());
#endif
}

bool TimingWheel::ScheduleImplement(Timer *timer, uint32 tick_count) {
  if(tick_count < TIMING_BASE_SIZE) {
    timer->timing_group_id_ = -1;
    this->base_.Schedule(timer, tick_count);
  } else if(tick_count < (1 << (TIMING_BASE_BIT + TIMING_GROUP_BIT))) {
    timer->left_base_tick_count_ = tick_count % TIMING_BASE_SIZE;
    timer->timing_group_id_ = 0;
    this->group_[0].Schedule(timer, tick_count);
    timer->left_base_tick_count_ += this->base_.GetCurrentTick();
  } else if(tick_count < (1 << (TIMING_BASE_BIT + 2 * TIMING_GROUP_BIT))) {
    timer->left_base_tick_count_ = tick_count % (TIMING_BASE_SIZE * TIMING_GROUP_SIZE);
    timer->timing_group_id_ = 1;
    this->group_[1].Schedule(timer, tick_count);
    timer->left_base_tick_count_ += this->group_[0].GetCurrentTick() * TIMING_BASE_SIZE;
    timer->left_base_tick_count_ += this->base_.GetCurrentTick();
  } else if(tick_count < (1 << (TIMING_BASE_BIT + 3 * TIMING_GROUP_BIT))) {
    timer->left_base_tick_count_ = tick_count % (TIMING_BASE_SIZE * TIMING_GROUP_SIZE * TIMING_GROUP_SIZE);
    timer->timing_group_id_ = 2;
    this->group_[2].Schedule(timer, tick_count);
    timer->left_base_tick_count_ += this->group_[1].GetCurrentTick() * TIMING_BASE_SIZE *
           TIMING_GROUP_SIZE;
    timer->left_base_tick_count_ += this->group_[0].GetCurrentTick() * TIMING_BASE_SIZE;
    timer->left_base_tick_count_ += this->base_.GetCurrentTick();
  } else {
    timer->left_base_tick_count_ = tick_count % (TIMING_BASE_SIZE * 
           TIMING_GROUP_SIZE * TIMING_GROUP_SIZE * TIMING_GROUP_SIZE);
    timer->timing_group_id_ = 3;
    this->group_[3].Schedule(timer, tick_count);
    timer->left_base_tick_count_ += this->group_[2].GetCurrentTick() * TIMING_BASE_SIZE *
           TIMING_GROUP_SIZE * TIMING_GROUP_SIZE;
    timer->left_base_tick_count_ += this->group_[1].GetCurrentTick() * TIMING_BASE_SIZE *
           TIMING_GROUP_SIZE;
    timer->left_base_tick_count_ += this->group_[0].GetCurrentTick() * TIMING_BASE_SIZE;
    timer->left_base_tick_count_ += this->base_.GetCurrentTick();
  }
  // Insert into the handler's timer list.
  timer->handler_->timer_list_.insert(std::make_pair(timer->id_, timer));
  return true;
}

void TimingWheel::Unschedule(uint32 id, TimerHandler *handler) {
  // Find timer by id.
  TimerHandler::HandlerTimerList::iterator iterator = handler->timer_list_.find(id);
  if(iterator != handler->timer_list_.end()) {
    Timer *timer = iterator->second;
    // Unschedule, so delete from handler's timer list.
    handler->timer_list_.erase(iterator);
    this->releases_.push_back(timer);
    timer->releasing_ = true;
  }
}

void TimingWheel::OnSchedule(const TimeTick &now) {
  uint64_t read_value;
  TimestampMillisecond timestamp(now);

#ifdef __CORE_KERNEL_TIMERFD__
  ::read(this->channel_.GetFd(), &read_value, sizeof(read_value));
  // Calculate timeout to compensate callback.
  uint64 distance = this->timestamp_.Distance(timestamp);
#else  // __CORE_KERNEL_TIMERFD__
  // Calculate timeout to compensate callback.
  uint64 distance = this->timestamp_.Distance(timestamp) + this->patch_msec_;
#endif  // __CORE_KERNEL_TIMERFD__

  uint32 count = distance / this->tick_msec_;

#ifdef _CORE_DEBUG
  if(count > 1)
    CoreLog(ERROR, "TimingWheel::OnSchedule count[%u]", count);
#endif  // _CORE_DEBUG

#ifdef __CORE_KERNEL_TIMERFD__
  if(count == 0)
    count = 1;
  this->timestamp_ = timestamp;
#else  // __CORE_KERNEL_TIMERFD__
  if(count > 0) {
    this->patch_msec_ = distance - count * this->tick_msec_;
    this->timestamp_ = timestamp;
  }
#endif  // __CORE_KERNEL_TIMERFD__

  //count *= 60;

  while(count-- > 0) {
    if(this->base_.OnSchedule() == true) {
      // Turn around, to make a carry.
      for(size_t pos = 0; pos < 4; ++pos) {
        if(this->group_[pos].OnSchedule() == false) break;
      }
    }
  }
  this->TimerRelease();
}

bool TimingWheel::Forward(int group_id, Timer *timer) {
  return this->ScheduleImplement(timer, timer->left_base_tick_count_);
}

bool TimingWheel::ResetTimer() {
  struct itimerspec new_value;
  bzero(&new_value, sizeof(new_value));
  new_value.it_value.tv_sec = 1;
  new_value.it_interval.tv_sec = this->tick_msec_ / 1000;
  new_value.it_interval.tv_nsec = (this->tick_msec_ % 1000) * 1000000;
#ifdef __CORE_KERNEL_TIMERFD__
  if(::timerfd_settime(this->channel_.GetFd(), 0, &new_value, NULL) != 0) {
    CoreLog(ERROR, "%s:%d (%s) timerfd_settime error.",
           __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
#else  // __CORE_KERNEL_TIMERFD__
  this->loop_->SetPollTimeout(this->tick_msec_ / 4);
#endif  // __CORE_KERNEL_TIMERFD__
  return true;
}

void TimingWheel::TimerRelease() {
  TimerReleaseList::iterator iterator = this->releases_.begin();
  for(; iterator != this->releases_.end(); ++iterator) {
    Timer *timer = *iterator;
    assert(timer);
    if(timer->timing_group_id_ == -1) {
      // In base, erase it.
      Timer::TimerList &bucket = base_.GetBucket(timer->timing_bucket_id_);
      if(timer->iterator_ != bucket.end())
        bucket.erase(timer->iterator_);
    } else if(timer->timing_group_id_ < 4) {
      // In group, erase it.
      Timer::TimerList &bucket = group_[timer->timing_group_id_].GetBucket(timer->timing_bucket_id_);
      if(timer->iterator_ != bucket.end())
        bucket.erase(timer->iterator_);
    } else {
      CoreLog(ERROR, "%s:%d (%s) Error timer(debug=[%s]) timing_group_id_.",
             __FILE__, __LINE__, __FUNCTION__, timer->debug_.c_str());
    }
    delete timer;
  }
  this->releases_.clear();
}

}  // namespace core

