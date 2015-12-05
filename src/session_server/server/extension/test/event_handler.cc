//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 16:54:22.
// File name: event_handler.cc
//
// Description:
// Define class EventHandler.
//

#include "session_server/server/extension/test/event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "global/global_packet.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"

namespace session {

namespace server {

namespace test {

EventHandler::EventHandler() {}
EventHandler::~EventHandler() {}

bool EventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->actor_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&EventHandler::OnActorLogin,
        this, _1, _2, _3, _4));

  // Follow 所有玩家等出事件
  this->actor_logout_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&EventHandler::OnActorLogout,
        this, _1, _2, _3, _4));

  return true;
}

void EventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->actor_login_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->actor_logout_);
}

void EventHandler::OnActorLogin(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message, size, channel_type 可能出错");
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  LOG_DEBUG("[事件测试] 玩家(%lu) 登陆 session 服务器", event->id_);

  // 发送扩展加载完成事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::TEST);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));
}

void EventHandler::OnActorLogout(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message, size, channel_type 可能出错");
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  LOG_DEBUG("[事件测试] 玩家(%lu) 登出 session 服务器", event->id_);
}

}  // namespace test

}  // namespace server

}  // namespace session

