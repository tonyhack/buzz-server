//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 17:44:54.
// File name: server_logout_event_handler.cc
//
// Description:
// Define class ServerLogoutEventHandler.
//

#include "session_server/server/extension/scene/server_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"
#include "session_server/server/extension/scene/scene_manager.h"

namespace session {

namespace server {

namespace scene {

ServerLogoutEventHandler::ServerLogoutEventHandler() {}
ServerLogoutEventHandler::~ServerLogoutEventHandler() {}

bool ServerLogoutEventHandler::Initialize() {
  // Follow 服务器登出事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_SERVER_LOGOUT, 0,
      event::ChannelType::CHANNEL_SYSTEM, boost::bind(
        &ServerLogoutEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ServerLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ServerLogoutEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSystemServerLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("参数 message, size, channel_type 可能出错");
    return ;
  }

  // 发送扩展加载完成事件
  event::EventSystemServerLogout *event = (event::EventSystemServerLogout *)message;

  // 只处理 GameServer 登出事件
  if(event->type_ != global::protocol::ServerType::GAME_SERVER) {
    return ;
  }

  // 删除 GameServer 下的所有场景
  SceneManager::GetInstance()->RemoveScenes(event->id_);
}

}  // namespace scene

}  // namespace server

}  // namespace session

