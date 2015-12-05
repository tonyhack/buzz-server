#ifndef __SESSION__SERVER__DAY__PASS__H
#define __SESSION__SERVER__DAY__PASS__H

#include "core/timer_handler.h"
#include "core/base/types.h"

namespace session {

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

}  // namespace session

#endif  // __SESSION__SERVER__DAY__PASS__H

