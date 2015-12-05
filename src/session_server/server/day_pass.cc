#include "session_server/server/day_pass.h"

#include "coresh/communicator.h"
#include "global/logging.h"
#include "session_server/server/session_server.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_guild_event_types.h"

namespace session {

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

  LOG_INFO("Daily clean timer[%u]", zero_stamp - now);

  SessionServerSingleton::GetInstance().AddTimer(0, (zero_stamp - now) * 1000, -1,
      this, "DayPass::AddDailyCleanTimer");
}

void DayPass::RemoveDailyCleanTimer() {
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_DAILY_CLEAN, this);
}

void DayPass::OnDailyCleanTimer() {
  // 发送玩家每日清理事件
  {
    event::EventActorDailyClean event;
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_DAILY_CLEAN , 0,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  // 发送公会每日清理事件
  {
    event::EventGuildDailyClean event;
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_GUILD_DAILY_CLEAN, 0,
        event::ChannelType::CHANNEL_SYSTEM, &event, sizeof(event));
  }
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

  LOG_INFO("Weekly clean timer[%u]", next_clean_time - now);

  SessionServerSingleton::GetInstance().AddTimer(0, (next_clean_time - now) * 1000, -1,
      this, "AddWeeklyCleanTimer");
}

void DayPass::RemoveWeeklyCleanTimer() {
  SessionServerSingleton::GetInstance().RemoveTimer(TIMER_ID_WEEKLY_CLEAN, this);
}

void DayPass::OnWeeklyCleanTimer() {
  // 发送玩家每周清理事件
  {
    event::EventActorWeeklyClean event;
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_WEEKLY_CLEAN , 0,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  // 发送公会每周清理事件
  {
    event::EventGuildWeeklyClean event;
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_GUILD_WEEKLY_CLEAN , 0,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
}

}  // namespace server

}  // namespace session

