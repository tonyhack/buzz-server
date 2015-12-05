#ifndef __GAME__SERVER__WELFARE__CHECKINS__ACTOR__H
#define __GAME__SERVER__WELFARE__CHECKINS__ACTOR__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_welfare_save_types.h"
#include "database_server/protocol/storage_welfare_load_types.h"
#include "entity/achieve_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/task.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

class CheckinsActor : public core::Noncopyable {
 typedef std::vector<core::int32> OnlineAwardItemVecs;
 typedef std::vector<core::int32> ReplenishCheckinsDays;
 public:
  CheckinsActor();
  ~CheckinsActor();

  bool InitializeActor(GameActor *actor, database::protocol::StorageWelfareField &field);
  void Finalize();

  void Save();
  void SaveActor(database::protocol::StorageWelfareField &field);

  core::uint64 GetActorID() {
    return this->actor_->GetActorID();
  }

  GameActor *GetActor() {
    return this->actor_;
  }
  
  // 定时清理
  void DailyClean(core::int32 days);
  // 检查每日签到
  inline bool CheckDailyCheckins(core::int32 day) {
    if(day <= 0 || day > (core::int32)sizeof(core::int32) * 8)  {
      return false;
    }
    return ((this->daily_checkins_ >> (day-1)) & 1) == 1 ? true : false;
  }
  // 设置每日签到通过
  inline void SetDailyCheckinsPass(core::int32 day) {
    if(day <= 0 || day > (core::int32)sizeof(core::int32) * 8) {
      return ;
    }
    this->daily_checkins_ = this->daily_checkins_ | (1 << (day-1));
  }
  // 检查当天签到奖励
  inline bool CheckCurrentCheckinsAward() {
    return this->daily_award_;
  }
  // 设置当天签到奖励
  inline void SetCurrentCheckinsAward(bool awarded) {
    this->daily_award_ = awarded;
  }
  // 检查普通玩累计家签到奖励(通过位来表示)
  inline bool CheckGeneralCheckins(core::int32 pos) {
    if(pos <= 0 || pos > (core::int32)sizeof(core::int32) * 8)  {
      return false;
    }
    return ((this->general_checkins_ >> (pos-1)) & 1) == 1 ? true : false;
  }
  // 设置普通玩家累计签到奖励(通过位表示)
  inline void SetGeneralCheckins(core::int32 pos) {
    if(pos <= 0 || pos > (core::int32)sizeof(core::int32) * 8)  {
      return ;
    }
    this->general_checkins_ = this->general_checkins_ | (1 << (pos-1));
  }
  // 检查VIP 玩家累计签到奖励()
  inline bool CheckVipCheckins(core::int32 pos) {
    if(pos <= 0 || pos > (core::int32)sizeof(core::int32) * 8)  {
      return false;
    }
    return ((this->vip_checkins_ >> (pos-1)) & 1) == 1 ? true : false;
  }
  // 设置VIP玩家累计签到奖励()
  inline void SetVipCheckins(core::int32 pos) {
    if(pos <= 0 || pos > (core::int32)sizeof(core::int32) * 8)  {
      return ;
    }
    this->vip_checkins_ = this->vip_checkins_ | (1 << (pos-1));
  }
  //检查每日在线奖励道具是否被领取()
  inline bool CheckOnlineAwardItem(core::int32 id) {
    if(this->online_award_items_.size() < (core::uint32)id) {
      return false;
    }
    return this->online_award_items_[id-1] == 0 ? false : true;
  }
  // 设置每日在线奖励道具
  inline void SetOnlineAwardItem(core::int32 id, core::int32 value) {
    if(this->online_award_items_.size() < (core::uint32)id) {
      return ;
    }
    this->online_award_items_[id-1] = value;
  }
  // 获取当前的在线时间
  core::int32 GetDailyTime();
  //提取玩家累计在线奖励资源
  bool PickOnlineResourceAward();
  // 签到莫一天
  bool CheckinsDays(core::int32 days);

  void Synchronize();

  core::int32 GetMonthDays();

  core::int32 GetMonthCheckinsNum();
  
  core::int32 GetDaysOfWeek();

  core::int32 GetDaysOfMonth();

  core::int32 GetPassDaysOpenService();

 private:

  // 玩家对象
  GameActor *actor_;
  // 每日签到记录
  core::int32 daily_checkins_;
  // 当日签到奖励
  bool daily_award_;
  // 签到普通奖励
  core::int32 general_checkins_;
  // VIP签到奖励
  core::int32 vip_checkins_;
  // 资源
  core::int32 last_month_resource_;
  // 本月资源
  core::int32 current_month_resource_;
  // 上一个月的资源是否已经领取
  bool last_month_award_resource_;
  // 本日累计的在线时间
  core::int32 daily_time_;
  // 本月累计在线时间
  core::int32 total_time_;
  // 上次保存日期
  core::int64 last_save_time_;
  // 已经补签的次数
  core::int32 replenish_checkins_num_;
  // 每日在线奖励道具
  OnlineAwardItemVecs online_award_items_;
  // 签到的天数
  ReplenishCheckinsDays replenish_days_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__CHECKINS__ACTOR__H
