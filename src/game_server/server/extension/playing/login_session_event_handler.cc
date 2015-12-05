#include "game_server/server/extension/playing/login_session_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_system_event_types.h"

namespace game {

namespace server {

namespace playing {

LoginSessionEventHandler::LoginSessionEventHandler() {}
LoginSessionEventHandler::~LoginSessionEventHandler() {}

bool LoginSessionEventHandler::Initialize() {
  // Follow 登陆 session_server 成功事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow( 
      event::EventType::EVENT_SYSTEM_LOGIN_SESSION, 0, event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&LoginSessionEventHandler::OnEvent, this, _1, _2, _3, _4));
  return true;
}

void LoginSessionEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void LoginSessionEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSystemLoginSession) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 向 session_server 请求地宫地图数据
  session::protocol::MessageMazeMapInfoRequest request;
  SessionChannel::SendMessage(request,
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_REQUEST);
}

}  // namespace playing

}  // namespace server

}  // namespace game

