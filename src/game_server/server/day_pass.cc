//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-07 16:13:31.
// File name: day_pass.cc
//
// Description:
// Define class DayPass.
//

#include "game_server/server/day_pass.h"

#include "coresh/communicator.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/event/game_actor_event_types.h"

namespace game {

namespace server {

DayPass::DayPass() {}
DayPass::~DayPass() {}

bool DayPass::Initialize() {
  this->AddDailyCleanTimer();
  this->AddWeeklyCleanTimer();
  return true;
}

void DayPass::Finalize() {
  this->RemoveDailyCleanTimer();
  this->RemoveWeeklyCleanTimer();
}

void DayPass::OnTimer(core::uint32 id) {
  if (TIMER_ID_DAILY_CLEAN == id) {
    this->OnDailyCleanTimer();
    this->RemoveDailyCleanTimer();
    this->AddDailyCleanTimer();
  } else if (TIMER_ID_WEEKLY_CLEAN == id) {
    this->OnWeeklyCleanTimer();
    this->RemoveWeeklyCleanTimer();
    this->AddWeeklyCleanTimer();
  }
}

void DayPass::AddDailyCleanTimer() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t zero_stamp = GlobalTimeTick::GetInstance()->GetZeroTime() + 24 * 60 * 60;

  LOG_DEBUG("Daily clean timer[%u]", zero_stamp - now);

  GameServerSingleton::GetInstance().AddTimer(0, (zero_stamp - now) * 1000, -1,
      this, "DayPass::AddDailyCleanTimer");
}

void DayPass::RemoveDailyCleanTimer() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_DAILY_CLEAN, this);
}

void DayPass::OnDailyCleanTimer() {
  GameActorManager::GetInstance()->DailyClean(1);
  // 发送玩家每日清理事件
  event::EventActorDailyClean event;
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_DAILY_CLEAN , 0,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void DayPass::AddWeeklyCleanTimer() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  time_t this_week_clean_time = GlobalTimeTick::GetInstance()->GetWeekTime(1, 0, 0, 0, now);
  time_t next_clean_time;
  if (now < this_week_clean_time) {
    next_clean_time = this_week_clean_time;
  } else {
    next_clean_time = this_week_clean_time + 7 * 24 * 60 * 60;
  }

  LOG_DEBUG("Weekly clean timer[%u]", next_clean_time - now);

  GameServerSingleton::GetInstance().AddTimer(0, (next_clean_time - now) * 1000, -1,
      this, "AddWeeklyCleanTimer");
}

void DayPass::RemoveWeeklyCleanTimer() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_WEEKLY_CLEAN, this);
}

void DayPass::OnWeeklyCleanTimer() {
  // 发送玩家每周清理事件
  event::EventActorWeeklyClean event;
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_WEEKLY_CLEAN , 0,
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

}  // namespace server

}  // namespace game

