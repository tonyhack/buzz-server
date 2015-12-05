//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-22 11:13:35.
// File name: timing_wheel_test.cc
//
// Description:
// Define class TimingWheelTest.
//

#include "test/timing_wheel/timing_wheel_test.h"

#include <stdio.h>

#include "core/configure.h"

namespace test {

namespace timing_wheel {

TimingWheelTest::TimingWheelTest() {}
TimingWheelTest::~TimingWheelTest() {}

bool TimingWheelTest::Initialize(core::EventLoop *loop) {
  if (loop == NULL) {
    return false;
  }

  this->loop_ = loop;

  // this->loop_->AddTimer(1, 24*60*60*1000, -1, this, "TimingWheelTest::Initialize");
  this->loop_->AddTimer(1, 60*1000, -1, this, "TimingWheelTest::Initialize");

  time_t now;
  now = time(NULL);
  tm *tm_now = localtime(&now);
  char current_time[32] = {0};
  strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", tm_now);

  printf("AddTimer now=%s\n", current_time);

  return true;
}

void TimingWheelTest::Finalize() {
  this->loop_ = NULL;
}

void TimingWheelTest::Run() {
}

void TimingWheelTest::OnTimer(core::uint32 id) {
  time_t now;
  now = time(NULL);
  tm *tm_now = localtime(&now);
  char current_time[32] = {0};
  strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", tm_now);

  printf("OnTimer now=%s\n", current_time);

  // this->loop_->AddTimer(1, 24*60*60*1000, -1, this, "TimingWheelTest::Initialize");
  this->loop_->AddTimer(1, 60*1000, -1, this, "TimingWheelTest::Initialize");
}

}  // namespace timing_wheel

}  // namespace test

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Need 1 params\n");
    return 0;
  }

  core::Configure::Initialize(argv[1]);

  core::EventLoop loop;
  if (loop.Initialize(true) == false) {
    printf("Initialize EventLoop failed.\n");
    return 0;
  }

  test::timing_wheel::TimingWheelTest test;
  if (test.Initialize(&loop) == false) {
    printf("Initialize TimingWheelTest failed.\n");
    return 0;
  }

  test.Run();

  loop.Loop();

  return 0;
}

