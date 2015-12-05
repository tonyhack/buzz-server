#ifndef __GAME__SERVER__SCENE__LOAD__CONSECUTIVE_KILL__CONFIGURE__H
#define __GAME__SERVER__SCENE__LOAD__CONSECUTIVE_KILL__CONFIGURE__H

#include <vector>
#include <string>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace scene {

class ConsecutiveKillCell {
 public:
  ConsecutiveKillCell() {}
  ~ConsecutiveKillCell() {}

  core::int32 num_;       // 连斩次数
  core::int32 span_time_; // 间隔时间
  core::int32 buff_id_;   // BUFF id
};

class LoadConsecutiveKillConfigure : public core::Noncopyable {
  typedef std::vector<ConsecutiveKillCell> ConsecutiveKillVec;

 public:
  LoadConsecutiveKillConfigure();
  ~LoadConsecutiveKillConfigure();

  bool LoadConfigure(const std::string &file);

  const ConsecutiveKillCell *GetNearCell(core::int32 kill_num) const;
  const ConsecutiveKillCell *GetCell(core::int32 kill_num) const;

 private:
  ConsecutiveKillVec consecutive_kills_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__LOAD__CONSECUTIVE_KILL__CONFIGURE__H

