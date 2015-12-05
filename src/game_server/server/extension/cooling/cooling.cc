//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-25 18:53:44.
// File name: cooling.cc
//
// Description:
// Define class Cooling.
//

#include "game_server/server/extension/cooling/cooling.h"

#include "game_server/server/extension/cooling/configure.h"
#include "game_server/server/global_time_tick.h"
#include "global/types.h"

namespace game {

namespace server {

namespace cooling {

Cooling::Cooling() : template_id_(0) {}
Cooling::~Cooling() {}

bool Cooling::Initialize(core::uint32 template_id) {
  this->end_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  this->template_id_ = template_id;

  return true;
}

void Cooling::Finalize() {}

bool Cooling::CheckEnd() const {
  core::TimestampMillisecond now;
  now.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  if(now.GetStamp() > this->end_time_.GetStamp()) {
    return true;
  }
  return false;
}

void Cooling::Start(core::int64 elapse, core::int64 add_value, core::int32 add_percent) {
  // 根据冷却ID得到配置
  const CoolingCell *configure =
    Configure::GetInstance()->GetCoolingCell(this->template_id_);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 冷却ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, this->template_id_);
    return ;
  }

  // 计算加成
  double percent = 1.0f + add_percent/global::kPercentDenom;
  core::int64 value = core::int64(configure->time_ * percent + add_value);

  // 是否已过冷却时间
  if(elapse >= value) {
    return ;
  }
  // 设置结束时间戳
  core::TimestampMillisecond now;
  now.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  this->end_time_ = now.GetStamp() + value - elapse;
}

void Cooling::End() {
  this->end_time_.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
}

core::uint64 Cooling::GetElapseTime() const {
  // 根据冷却ID得到配置
  const CoolingCell *configure =
    Configure::GetInstance()->GetCoolingCell(this->template_id_);
  if(configure == NULL) {
    global::LogError("%s:%d (%s) 冷却ID(%u) 不存在",
        __FILE__, __LINE__, __FUNCTION__, this->template_id_);
    return 0;
  }
  core::TimestampMillisecond now;
  now.Stamp(GlobalTimeTick::GetInstance()->TimeTick());
  if(this->end_time_.GetStamp() > now.GetStamp()) {
    core::uint64 left_time = this->end_time_.GetStamp() - now.GetStamp();
    if(left_time < configure->time_) {
      return configure->time_ - left_time;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

}  // namespace cooling

}  // namespace server

}  // namespace game

