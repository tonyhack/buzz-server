//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 16:10:10.
// File name: day_pass.h
//
// Description:
// Define class DayPass.
//

#ifndef __GAME__SERVER__DAY__PASS__H
#define __GAME__SERVER__DAY__PASS__H

#include "core/timer_handler.h"
#include "core/base/types.h"

namespace game {

namespace server {

class DayPass : public core::TimerHandler {
  enum {
    TIMER_ID_DAILY_CLEAN = 0,
    TIMER_ID_WEEKLY_CLEAN,
  };

 public:
  DayPass();
  virtual ~DayPass();

  bool Initialize();
  void Finalize();

  virtual void OnTimer(core::uint32 id);

 private:
  void AddDailyCleanTimer();
  void RemoveDailyCleanTimer();
  void OnDailyCleanTimer();

  void AddWeeklyCleanTimer();
  void RemoveWeeklyCleanTimer();
  void OnWeeklyCleanTimer();
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__DAY__PASS__H

