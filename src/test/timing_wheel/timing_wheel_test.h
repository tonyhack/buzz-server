//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-22 11:32:41.
// File name: timing_wheel_test.h
//
// Description:
// Define class TimingWheelTest.
//

#ifndef __TEST__TIMING__WHEEL__TIMING__WHEEL__TEST__H
#define __TEST__TIMING__WHEEL__TIMING__WHEEL__TEST__H

#include "core/event_loop.h"
#include "core/timer_handler.h"

namespace test {

namespace timing_wheel {

class TimingWheelTest : core::TimerHandler {
 public:
  TimingWheelTest();
  virtual ~TimingWheelTest();

  bool Initialize(core::EventLoop *loop);
  void Finalize();

  void Run();

  virtual void OnTimer(core::uint32 id);

 private:
  core::EventLoop *loop_;
};

}  // namespace timing_wheel

}  // namespace test

#endif  // __TEST__TIMING__WHEEL__TIMING__WHEEL__TEST__H

