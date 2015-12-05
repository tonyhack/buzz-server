//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 17:52:24.
// File name: cooling.h
//
// Description:
// Define class Cooling.
//

#ifndef __GAME__SERVER__COOLING__COOLING__H
#define __GAME__SERVER__COOLING__COOLING__H

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace cooling {

class CoolingRole;

class Cooling : public core::Noncopyable {
 public:
  Cooling();
  ~Cooling();

  bool Initialize(core::uint32 template_id);
  void Finalize();

  // 是否冷却结束
  bool CheckEnd() const;

  void Start(core::int64 elapse, core::int64 add_value = 0, core::int32 add_percent = 0);
  void End();

  core::uint64 GetElapseTime() const;

  // 得到冷却结束时间
  inline const core::TimestampMillisecond &GetEndTime() const {
    return this->end_time_;
  }

 private:
  // 冷却结束的时间戳
  core::TimestampMillisecond end_time_;
  core::uint32 template_id_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__H

