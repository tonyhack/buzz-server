
#ifndef __GAME__SERVER__WELFARE__CONFIGURE__H
#define __GAME__SERVER__WELFARE__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/welfare/load_daily_checkins_configure.h"
#include "game_server/server/extension/welfare/load_checkins_configure.h"
#include "game_server/server/extension/welfare/load_online_configure.h"
#include "game_server/server/extension/welfare/load_task_configure.h"

namespace game {

namespace server {

namespace welfare {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  const AchieveTaskCell *GetAchieveCell(core::int32 id) const {
    return this->load_achieve_.GetCell(id);
  }

  const LoadTaskConfigure::AchieveMap &GetAchievesCell() const {
    return this->load_achieve_.GetCells();
  }

  const DailyCheckinsCell *GetWeekCell(core::int32 day) const {
    return this->load_daily_checkins_.GetCell(day);
  }

  const CheckinsCell *GetCell(core::int32 day) const {
    return this->load_checkins_.GetCell(day);
  }

  OnlineCell *RandCell(core::int32 online_time) {
    return this->load_online_.RandCell(online_time);
  }

  core::int32 GetOnlineToPos(core::int32 online_time) {
    return this->load_online_.GetOnlineToPos(online_time);
  }

  core::int32 condition_extend_number_;
  core::int32 condition_initial_number_;

  core::int32 task_extend_number_;
  core::int32 task_initial_number_;

 private:
  Configure() {}
  ~Configure() {}

  LoadTaskConfigure load_achieve_;
  // 累积签到奖励
  LoadCheckinsConfigure load_checkins_;
  // 每日签到奖励
  LoadDailyCheckinsConfigure load_daily_checkins_;
  // 每日在线奖励
  LoadOnlineConfigure load_online_;

};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__CONFIGURE__H

