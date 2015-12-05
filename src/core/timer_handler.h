//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-04 14:43:05.
// File name: timer_handler.h
//
// Description:
// Define class TimerHandler.
//

#ifndef __CORE__TIMER__HANDLER__H
#define __CORE__TIMER__HANDLER__H

#include <map>

#include "core/base/types.h"

namespace core {

class Timer;

class TimerHandler {
 public:
  enum { INFINITE_COUNT = -1, };
  typedef std::map<uint32, Timer *> HandlerTimerList;

  TimerHandler() {}
  virtual ~TimerHandler() {}
  
  virtual void OnTimer(uint32 id) = 0;
  
  HandlerTimerList timer_list_;
};

}  // namespace core

#endif  // __CORE__TIMER__HANDLER__H

