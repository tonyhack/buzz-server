//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:39:05.
// File name: event_loop.h
//
// Description:
// Define class EventLoop, It's a reactor.
// With Epoll, EventLoop uses to dispose a large number of channel(I/O event)
// in a thread.
//

#ifndef __CORE__EVENT__LOOP__h
#define __CORE__EVENT__LOOP__h

#include <sys/epoll.h>

#include <string>
#include <vector>

#include "core/base_loop.h"
#include "core/callbacks.h"
#include "core/channel.h"
#include "core/base/mutex.h"
#include "core/base/thread.h"
#include "core/base/time_tick.h"

namespace core {

class Epoller;
class EventControl;
class EventHandler;
class SignalSet;
class TimingWheel;
class TimerHandler;

class EventLoop : public BaseLoop {
 public:
  typedef std::vector<EventLoopCallback> FunctorList;
  typedef std::vector<EventLoopTimeCallback> TimeFunctorList;
  typedef std::vector<struct epoll_event> ChannelList;

  explicit EventLoop(int poll_timeout_msec_ = -1);
  virtual ~EventLoop();

  virtual bool Initialize(bool timer);
  virtual void Finalize();

  virtual void Loop();

  // Wakeup.
  virtual void Wakeup();

  inline void SetPollTimeout(int poll_timeout_msec = -1) {
    this->poll_timeout_msec_ = poll_timeout_msec;
  }

  // Timer.
  void AddTimer(uint32 id, uint32 msecs, int call_times,
         TimerHandler *handler, const std::string &debug);
  void RemoveTimer(uint32 id, TimerHandler *handler);

  // Event.
  void SetEvent(const EventHandler *handler);

  // Let epoll related to a channel.
  void AddChannel(Channel* channel);
  void RemoveChannel(Channel* channel);
  void UpdateChannel(Channel* channel);

  bool CheckInLoopThread() const { return this->thread_id_ == current_thread::tid(); }

  // Wakeup.
  void WakeupCallback(const EventLoopCallback &callback, bool callback_immediate = true);

  // Callback at the tail of loop.
  void TailCallback(const EventLoopCallback &callback);

  // Callback attach time tick.
  void TimeCallback(const EventLoopTimeCallback &callback);

  // Get time.
  inline const TimeTick &GetTimeTick() const { return this->time_; }

  // Get wakeup data.
  void HandleRead(const TimeTick &time);

 private:
  inline void DoCallbacks() {
    this->DoTailCallbacks();
    this->DoWakeupCallbacks();
  }

  // Wakeup to do.
  void DoWakeupCallbacks();

  // callback at the tail.
  void DoTailCallbacks();

  // callback with a time.
  void DoTimeCallbacks(const TimeTick &time_tick);

  // Thread id.
  pid_t thread_id_;

  // Epoller.
  Epoller *epoller_;
  // Epoll gives active I/O channels.
  ChannelList active_channels_;

  // Run once at the end of loop.
  FunctorList wakeup_callbacks_;

  // Run at the end of loop.
  FunctorList tail_callbacks_;

  // Run at the end of loop.
  TimeFunctorList time_callbacks_;

  // Mutex.
  mutable Mutex wakeup_mutex_;
  mutable Mutex tail_mutex_;
  mutable Mutex time_mutex_;

  // Time tick.
  TimeTick time_;

  // Timing wheel.
  TimingWheel *wheel_;

  // Event control.
  EventControl *event_;

  int poll_timeout_msec_;

  // wakeup.
  int wakeup_fd_[2];
  Channel wakeup_channel_;

  bool timer_;
};

}  // namespace core

#endif  // __CORE__EVENT__LOOP__h

