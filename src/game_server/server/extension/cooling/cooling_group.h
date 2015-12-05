//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 22:14:15.
// File name: cooling_group.h
//
// Description:
// Define class CoolingGroup.
//

#ifndef __GAME__SERVER__COOLING__COOLING__GROUP__H
#define __GAME__SERVER__COOLING__COOLING__GROUP__H

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace cooling {

class CoolingGroup : public core::Noncopyable {
 public:
  CoolingGroup();
  ~CoolingGroup();

  bool Initialize(core::uint32 id);
  void Finalize();

  // 是否结束
  bool CheckEnd() const;

  void Start(const core::TimestampMillisecond &end_time);
  void End();

  inline const core::TimestampMillisecond &GetEndTime() const {
    return this->end_time_;
  }

 private:
  core::uint32 id_;
  core::TimestampMillisecond end_time_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__COOLING__GROUP__H

