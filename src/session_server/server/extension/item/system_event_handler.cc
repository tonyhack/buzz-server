
#include "session_server/server/extension/item/system_event_handler.h"

#include <boost/bind.hpp>

#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"
#include "session_server/server/extension/item/item_extension.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace item {

SystemEventHandler::SystemEventHandler() {}
SystemEventHandler::~SystemEventHandler() {}

bool SystemEventHandler::Initialize() {
  this->conn_server_login_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_SERVER_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SYSTEM, boost::bind(&SystemEventHandler::OnEventSystemServerLogin,
        this, _1, _2, _3, _4));

  return true;
}

void SystemEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_server_login_);
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

  ItemExtension::GetInstance()->SynchronizeMallConfig(event->id_);
}

}  // namespace item

}  // namespace server

}  // namespace session

