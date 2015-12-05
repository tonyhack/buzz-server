//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 01:23:24.
// File name: extension_load_event_handler.cc
//
// Description:
// Define class ExtensionLoadEventHandler.
//

#include "game_server/server/extension_load_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_system_event_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

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

  event::EventActorExtensionLoadFinish *event = (event::EventActorExtensionLoadFinish *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 获取 GameActor失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 完成扩展加载
  actor->GetLoadProgress().LoadExtensionFinish((ExtensionType::type)event->extension_type_);

  if(actor->GetLoadProgress().LoadFinish() == false) {
    return ;
  }

  global::LogDebug("玩家(%lu) 完成了登陆，发送EVENT_ACTOR_LOAD_FINISH事件", event->id_);

  actor->OnExtentionLoadFinish();

  // 发送加载完成事件
  event::EventActorLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorLoadFinish));

  // 发送登陆成功到会话服务器
  this->FinishLogin(event->id_, actor->GetName(), actor->GetJumpLogin());

  time_t now_time = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();

  // 如果下线时间在今天0点之前，需要清理
  time_t zero_time = GlobalTimeTick::GetInstance()->GetZeroTime();
  if(zero_time >= actor->GetLastLogoutTime()) {
    event::EventActorDailyClean clean_event;
    // 获取经过多少天数
    if(actor->GetLastLogoutTime() <= 0) {
      clean_event.__set_days_(0);
    } else {
      clean_event.__set_days_(global::GetNatureDays(
            actor->GetLastLogoutTime(), now_time));
    }
    clean_event.__set_actor_(event->id_);
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

void ExtensionLoadEventHandler::FinishLogin(core::uint64 actor,
    const std::string &name, bool jump_scene) {
  session::protocol::GameMessageActorLoginResponse message;
  message.__set_actor_id_(actor);
  message.__set_logined_(true);
  message.__set_jump_scene_(jump_scene);
  SessionChannel::SendMessage(message,
      session::protocol::GameMessageType::MESSAGE_ACTOR_LOGIN_RESPONSE);
}

}  // namespace server

}  // namespace game


