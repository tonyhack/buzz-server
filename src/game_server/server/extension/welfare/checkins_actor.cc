
#include "game_server/server/extension/welfare/checkins_actor.h"

#include "entity/welfare_types.h"
#include "game_server/server/global_time_tick.h"
#include "gateway_server/protocol/gateway_welfare_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/configure/configure.h"
#include "global/common_functions.h"
#include "global/server_configure.h"

namespace game {

namespace server {

namespace welfare {

CheckinsActor::CheckinsActor() {}

CheckinsActor::~CheckinsActor() {}

bool CheckinsActor::InitializeActor(GameActor *actor, database::protocol::StorageWelfareField &field) {
  if(actor == NULL) {
    LOG_ERROR("玩家对象没有找到");
    return false;
  }
  this->actor_ = actor;
  this->daily_checkins_ = field.daily_checkins_;
  this->daily_award_ = field.daily_award_;
  this->general_checkins_ = field.general_checkins_;
  this->vip_checkins_ = field.vip_checkins_;
  global::StringSplit(field.online_award_items_.c_str(), ":", this->online_award_items_);
  if(this->online_award_items_.size() == 0 || this->online_award_items_.size() != (core::uint32)entity::CheckinsConfigType::ONLINE_LIST) {
    this->online_award_items_.resize(entity::CheckinsConfigType::ONLINE_LIST);
  }
  this->last_month_resource_ = field.last_month_resource_;
  this->last_month_award_resource_ = field.last_month_award_resource_;
  this->current_month_resource_ = field.current_month_resource_;
  this->daily_time_ = field.daily_time_;
  this->total_time_ = field.total_time_;
  this->replenish_checkins_num_ = field.replenish_checkins_num_;
  this->last_save_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(this->actor_->GetLastLogoutTime() == 0) {
    this->last_month_award_resource_ = true;
  }
  this->replenish_days_.clear();
  global::StringSplit(field.respenish_days_.c_str(), ":", this->replenish_days_);

  return true;
}

void CheckinsActor::Finalize() {
  this->actor_ = NULL;
  this->daily_checkins_ = 0;
  this->daily_award_ = false;
  this->general_checkins_ = 0;
  this->vip_checkins_ = 0;
  this->online_award_items_.clear();
  this->last_month_resource_ = 0;
  this->last_month_award_resource_ = false;
  this->current_month_resource_ = 0;
  this->daily_time_ = 0;
  this->total_time_ = 0;
  this->replenish_checkins_num_ = 0;
  this->replenish_days_.clear();
}

void CheckinsActor::SaveActor(database::protocol::StorageWelfareField &field) {
  // 计算在线时间
  this->GetDailyTime();

  field.__set_current_month_resource_(this->current_month_resource_);
  field.__set_daily_award_(this->daily_award_);
  field.__set_daily_checkins_(this->daily_checkins_);
  field.__set_general_checkins_(this->general_checkins_);
  field.__set_last_month_award_resource_(this->last_month_award_resource_);
  field.__set_last_month_resource_(this->last_month_resource_);
  field.__set_daily_time_(this->daily_time_);
  if(this->online_award_items_.size() == 0) {
    LOG_ERROR("在线奖励道具列表失败");
    return ;
  }
  field.online_award_items_ += global::ToString(this->online_award_items_[0]);
  for(size_t i = 1; i < this->online_award_items_.size(); ++i) {
    field.online_award_items_ += ":";
    field.online_award_items_ += global::ToString(this->online_award_items_[i]);
  }
  field.__set_total_time_(this->total_time_);
  field.__set_vip_checkins_(this->vip_checkins_);
  field.__set_replenish_checkins_num_(this->replenish_checkins_num_);
  if(this->replenish_days_.size() != 0) {
    field.respenish_days_ += global::ToString(this->replenish_days_[0]);
    for(size_t i = 1; i < this->replenish_days_.size(); ++i) {
      field.respenish_days_ += ":";
      field.respenish_days_ += global::ToString(this->replenish_days_[i]);
    }
  }
}

core::int32 CheckinsActor::GetDailyTime() {
  // 计算在线时间
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  core::int32 front_compare = this->daily_time_/3600;
  this->daily_time_ += now - this->last_save_time_;
  core::int32 after_compare = this->daily_time_/3600;
  if(front_compare != after_compare) {
    if(after_compare <= MISC_CONF()->max_online_time_) {
      core::int32 passDays = this->GetPassDaysOpenService();
      // 过了一小时
      if(passDays <= MISC_CONF()->online_time_level1_) {
        this->current_month_resource_ += MISC_CONF()->online_time_award1_;
      } else if(passDays > MISC_CONF()->online_time_level1_ && passDays <= MISC_CONF()->online_time_level2_) {
        this->current_month_resource_ += MISC_CONF()->online_time_award2_;
      } else {
        this->current_month_resource_ += MISC_CONF()->online_time_award3_;
      }
    }
  }
  this->total_time_ += now - this->last_save_time_;

  this->last_save_time_ = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  return this->daily_time_;
}

core::int32 CheckinsActor::GetPassDaysOpenService() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t span = now - global::ServerConfigureSingleton::GetInstance().GetOpenServiceTime();
  return span/(3600*24) + (span%(3600*24) != 0 ? 1 : 0);
}

void CheckinsActor::Synchronize() {
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象没有找到");
  }
  gateway::protocol::MessageCheckinsSynchronization message;
  core::int32 dayofmonth = this->GetDaysOfMonth();
  for(core::int32 i = 1; i <= dayofmonth; ++i) {
    message.checkins_.push_back(this->CheckDailyCheckins(i)); 
  }
  message.__set_day_award_(this->daily_award_);
  for(core::int32 i = 1; i <= entity::CheckinsConfigType::GENERAL_CHECKINS_LIST; ++i) {
    message.general_awards_.push_back(this->CheckGeneralCheckins(i));
  }
  for(core::int32 i = 1; i <= entity::CheckinsConfigType::VIP_CHECKINS_LIST; ++i) {
    message.vip_awards_.push_back(this->CheckVipCheckins(i));
  }
  message.online_awrads_ = this->online_award_items_;
  message.__set_daily_online_time_(this->daily_time_);
  message.__set_total_time_(this->total_time_);
  message.__set_resource_awards_(this->last_month_award_resource_);
  message.__set_replenish_checkins_num_(this->replenish_checkins_num_);
  message.__set_month_resource_(this->current_month_resource_);
  message.__set_last_month_resource_(this->last_month_resource_);
  message.__set_respenish_checkins_days_(this->replenish_days_);
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_CHECKINS_SYNCHRONIZATION);
}

core::int32 CheckinsActor::GetMonthDays() {
  // 获取当前的年份 
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  tm * tm = localtime(&now);
  if(tm == NULL) {
    LOG_ERROR("获取当前年份失败");
    return 0;
  }
  core::int32 add = 0;
  if((tm->tm_year%4 == 0 && tm->tm_year%100 != 0) || tm->tm_year%400 == 0) {
    add += 1;
  }
  switch(tm->tm_mon) {
    case 0:
      return 31;
      break;
    case 1:
      return 28 + add;
      break;
    case 2:
      return 31;
      break;
    case 3:
      return 30;
      break;
    case 4:
      return 31;
      break;
    case 5: 
      return 30;
      break;
    case 6:
      return 31;
      break;
    case 7:
      return 31;
      break;
    case 8:
      return 30;
      break;
    case 9:
      return 31;
      break;
    case 10:
      return 30;
      break;
    case 11:
      return 31;
      break;
    default:
      break;
  }
  return 0;
}

core::int32 CheckinsActor::GetDaysOfWeek() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  tm * tm = localtime(&now);
  if(tm == NULL) {
    LOG_ERROR("获取当前年份失败");
    return 0;
  }
  return tm->tm_wday;
}

core::int32 CheckinsActor::GetDaysOfMonth() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  tm * tm = localtime(&now);
  if(tm == NULL) {
    LOG_ERROR("获取当前年份失败");
    return 0;
  }
  return tm->tm_mday;
}

core::int32 CheckinsActor::GetMonthCheckinsNum() {
  core::int32 num = 0;
  for(core::int32 i = 1; i <= this->GetMonthDays(); ++i) {
    if(this->CheckDailyCheckins(i) == true) {
      ++num;
    }
  }
  return num;
}

bool CheckinsActor::PickOnlineResourceAward() {
  if(this->last_month_award_resource_ == true) {
    LOG_ERROR("上月的资源奖励已经被领取");
    return false;
  }
  // 奖励资源
  if(this->actor_ == NULL) {
    LOG_ERROR("游戏玩家没有找到");
    return false;
  }
  this->actor_->AddResource(entity::ResourceID::GOLD, this->last_month_resource_);
  this->last_month_resource_ = 0;
  this->last_month_award_resource_ = true;
  return true;
}

void CheckinsActor::DailyClean(core::int32 days) {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  tm * tm = localtime(&now);
  if(tm->tm_mday == 1 && days > 0) {
    // 过月
    this->daily_checkins_ = 0;
    this->general_checkins_ = 0;
    this->vip_checkins_ = 0;
    this->last_month_resource_ = this->current_month_resource_;
    this->current_month_resource_ = 0;
    this->replenish_checkins_num_ = 0;
    this->replenish_days_.clear();
    this->last_month_award_resource_ = false;
  }
  if(days > 0) {
    // 过天
    this->online_award_items_.clear();
    this->online_award_items_.resize(entity::CheckinsConfigType::ONLINE_LIST);
    this->daily_award_ = false;
    this->daily_time_ = 0;
    if(this->GetDaysOfMonth() >= MISC_CONF()->pick_awards_limit_time_) {
      this->last_month_award_resource_ = true;
      this->last_month_resource_ = 0;
    }
  }
  this->Synchronize();
}

bool CheckinsActor::CheckinsDays(core::int32 days) {
  if(this->GetDaysOfMonth() < days) {
    LOG_ERROR("当前的天数不能签到(%d)", days);
    return false;
  } else if(this->GetDaysOfMonth() == days) {
    this->SetDailyCheckinsPass(days);
    return true;
  } else {
    core::int32 vip_num = 0;
    const global::configure::VipCell *vip_cell =
      VIP_CONF()->GetCell((entity::VipType::type)this->actor_->GetAttribute(entity::ActorAoiFields::VIP),
          entity::VipPrivilegeType::CHECKIN_UP_NUM);
    if(vip_cell != NULL) {
      vip_num = vip_cell->value1_;
    }
    if(this->replenish_checkins_num_ >= MISC_CONF()->replenish_checkins_num_ + vip_num) {
      LOG_ERROR("玩家补签到次数不够，无法补签");
      return false;
    }
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    tm *tm = localtime(&now);
    time_t checkins_time  = now - (tm->tm_mday - days - 1)*24*3600;
    if(checkins_time <= this->actor_->GetCreateTime()) {
      LOG_ERROR("时间错误");
      return false;
    }
    this->SetDailyCheckinsPass(days);
    this->replenish_checkins_num_ += 1;
    this->replenish_days_.push_back(days);
    return true;
  }
  return false;
}

}  // namespace welfare

}  // namespace server

}  // namespace game
