//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 22:19:58.
// File name: cooling_group.cc
//
// Description:
// Define class CoolingGroup.
//

#include "game_server/server/extension/cooling/cooling_group.h"

#include "game_server/server/extension/cooling/configure.h"
#include "game_server/server/extension/cooling/cooling_configure.h"
#include "game_server/server/global_time_tick.h"

namespace game {

namespace server {

namespace cooling {

CoolingGroup::CoolingGroup() : id_(0) {}

CoolingGroup::~CoolingGroup() {}

bool CoolingGroup::Initialize(core::uint32 id) {
  this->id_ = id;

  const GroupCell *cell = Configure::GetInstance()->GetGroupCell(this->id_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 冷却组(%u)配置不存在",
        __FILE__, __LINE__, __FUNCTION__, this->id_);
    return false;
  }

  this->end_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  return true;
}

void CoolingGroup::Finalize() {}

bool CoolingGroup::CheckEnd() const {
  core::TimestampMillisecond now;
  now.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  if(now.GetStamp() > this->end_time_.GetStamp()) {
    return true;
  }
  return false;
}

void CoolingGroup::Start(const core::TimestampMillisecond &end_time) {
  this->end_time_ = end_time;
}

void CoolingGroup::End() {
  this->end_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
}

}  // namespace cooling

}  // namespace server

}  // namespace game

