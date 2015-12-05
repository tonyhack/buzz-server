//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-23 10:39:51.
// File name: system_event_handler.cc
//
// Description:
// Define class SystemEventHandler.
//

#include "session_server/server/extension/playing/system_event_handler.h"

#include <boost/bind.hpp>

#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"
#include "session_server/server/extension/playing/playing_load_balance.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace playing {

SystemEventHandler::SystemEventHandler() {}
SystemEventHandler::~SystemEventHandler() {}

bool SystemEventHandler::Initialize() {
  this->conn_server_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_SERVER_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SYSTEM, boost::bind(&SystemEventHandler::OnEventSystemServerLogin,
        this, _1, _2, _3, _4));

  this->conn_server_logout_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_SERVER_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SYSTEM, boost::bind(&SystemEventHandler::OnEventSystemServerLogout,
        this, _1, _2, _3, _4));

  return true;
}

void SystemEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_server_login_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_server_logout_);
}

void SystemEventHandler::OnEventSystemServerLogin(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventSystemServerLogin) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  event::EventSystemServerLogin *event = (event::EventSystemServerLogin *)message;

  if(event->type_ != global::protocol::ServerType::GAME_SERVER) {
    return ;
  }

  PlayingLoadBalance::GetInstance()->OnGameServerLogin(event->id_);
}

void SystemEventHandler::OnEventSystemServerLogout(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventSystemServerLogout) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return ;
  }

  event::EventSystemServerLogout *event = (event::EventSystemServerLogout *)message;

  if(event->type_ != global::protocol::ServerType::GAME_SERVER) {
    return ;
  }

  PlayingLoadBalance::GetInstance()->OnGameServerLogout(event->id_);
}

}  // namespace playing

}  // namespace server

}  // namespace session

