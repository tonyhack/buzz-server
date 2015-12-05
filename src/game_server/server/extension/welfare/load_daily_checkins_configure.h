#ifndef __GAME__SERVER__WELFARE__LOAD__DAILY_CHECKINS__CONFIGURE__H
#define __GAME__SERVER__WELFARE__LOAD__DAILY_CHECKINS__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/welfare/load_task_configure.h"

namespace game {

namespace server {

namespace welfare {

class DailyCheckinsCell {
 public:
  core::int32 day_;
  Award award_[5];
};

class LoadDailyCheckinsConfigure : public core::Noncopyable {
 public:
   LoadDailyCheckinsConfigure();
   ~LoadDailyCheckinsConfigure();

   bool LoadConfigure(const std::string &file);

   const DailyCheckinsCell *GetCell(core::int32 id) const {
     if(id >= 0 && id < 7) {
       return &(this->dailys_[id]);
     }
     return NULL;
   }

 private:
   // 七天的配置
   std::vector<DailyCheckinsCell> dailys_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__LOAD__DAILY_CHECKINS__CONFIGURE__H

