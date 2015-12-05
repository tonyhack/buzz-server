#ifndef __GAME__SERVER__WELFARE__LOAD___CHECKINS__CONFIGURE__H
#define __GAME__SERVER__WELFARE__LOAD___CHECKINS__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/welfare/load_task_configure.h"

namespace game {

namespace server {

namespace welfare {

class CheckinsCell {
 public:
  core::int32 id_;
  core::int32 sort_;
  Award award_[5];
  Award vip_award_[5];
};

class LoadCheckinsConfigure : public core::Noncopyable {
  typedef std::map<core::int32, CheckinsCell> CheckinsMap;
 public:
  LoadCheckinsConfigure();
  ~LoadCheckinsConfigure();

  bool LoadConfigure(const std::string &file);

  const CheckinsCell *GetCell(core::int32 day) const {
    CheckinsMap::const_iterator itr = this->checkins_.find(day);
    if(itr != this->checkins_.end()) {
      return &(itr->second);
    }
    return NULL;
  }

 private:
  CheckinsMap checkins_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__LOAD___CHECKINS__CONFIGURE__H

