//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 17:06:35.
// File name: extension_load_event_handler.cc
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#include "session_server/server/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "session_server/server/global_time_tick.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace session {

namespace server {

ExtensionLoadEventHandler::ExtensionLoadEventHandler() {}
ExtensionLoadEventHandler::~ExtensionLoadEventHandler() {}

bool ExtensionLoadEventHandler::Initialize() {
  // Follow 所有玩家扩展模块加载完成事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ExtensionLoadEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ExtensionLoadEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ExtensionLoadEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorExtensionLoadFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorExtensionLoadFinish *event =
    (event::EventActorExtensionLoadFinish *)message;

  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu) 获取 SessionActor 失败", event->id_);
    return ;
  }

  // 完成扩展加载
  actor->GetLoadProgress().LoadExtensionFinish(
      (ExtensionType::type)event->extension_type_);

  // 扩展全部加载完成，发送事件
  if(actor->GetLoadProgress().LoadFinish() == true) {
    LOG_INFO("玩家(%lu) 完成了登陆，发送 发送EVENT_ACTOR_LOAD_FINISH 事件", event->id_);
    actor->OnSessionLoadFinish();
    event::EventActorLoadFinish load_finish;
    load_finish.__set_id_(event->id_);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_LOAD_FINISH, event->id_,
        event::ChannelType::CHANNEL_ACTOR, &load_finish,
        sizeof(event::EventActorLoadFinish));
    // 如果下线时间在今天0点之前，需要清理
    time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
    time_t now_time = SessionServerSingleton::GetInstance().GetTimeTick().GetRealSec();
    if(zero_time >= actor->GetLastLogoutTime()) {
      event::EventActorDailyClean clean_event;
      clean_event.__set_actor_(event->id_);
      clean_event.__set_daily_(global::GetNatureDays(actor->GetLastLogoutTime(), now_time));
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_DAILY_CLEAN, event->id_,
          event::ChannelType::CHANNEL_ACTOR, &clean_event, sizeof(clean_event));
    }
    // 检查是否需要进行每周清理
    time_t this_week_clean_time = GlobalTimeTick::GetInstance()->GetWeekTime(1, 0, 0, 0, now_time);
    if (actor->GetLastLogoutTime() < this_week_clean_time &&
        now_time >= this_week_clean_time) {
      event::EventActorWeeklyClean clean_event;
      clean_event.__set_actor_(event->id_);
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_WEEKLY_CLEAN, event->id_,
          event::ChannelType::CHANNEL_ACTOR, &clean_event, sizeof(clean_event));
    }
  }

}

}  // namespace server

}  // namespace session

