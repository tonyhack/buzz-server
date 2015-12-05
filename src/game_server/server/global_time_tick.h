//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-12 10:22:06.
// File name: global_time_tick.h
//
// Description:
// Define class GlobalTimeTick.
//

#ifndef __GAME__SERVER__GLOBAL__TIME__TICK__H
#define __GAME__SERVER__GLOBAL__TIME__TICK__H

#include "core/base/time_tick.h"
#include "core/base/types.h"
#include "game_server/server/game_server.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GlobalTimeTick : public global::SingletonFactory<GlobalTimeTick> {
  friend class global::SingletonFactory<GlobalTimeTick>;

 public:
  inline const core::TimeTick &TimeTick() {
    return GameServerSingleton::GetInstance().GetTimeTick();
  }

  inline time_t GetZeroTime() const {
    time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
    time_t zero_stamp = 0;
    struct tm *tm = NULL;

    tm = localtime(&now);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;

    return mktime(tm);
  }

  inline time_t GetWeekTime(core::int32 week_day, core::int32 hour,
      core::int32 minute, core::int32 second, time_t now = 0) const {
    if(0 == now) {
      now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
    }
    struct tm *tm = localtime(&now);

    time_t week_begin_time = now - (tm->tm_wday * 24 * 60 * 60 +
        tm->tm_hour * 60 * 60 + tm->tm_min * 60 + tm->tm_sec);

    return week_begin_time + week_day * 24 * 60 * 60 +
        hour * 60 * 60 + minute * 60 + second;
  }

 private:
  GlobalTimeTick() {}
  ~GlobalTimeTick() {}
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GLOBAL__TIME__TICK__H

