#include "game_server/server/extension/playing/session_maze_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/playing/maze_map_manager.h"
#include "game_server/server/extension/playing/playing_actor_manager.h"

namespace game {

namespace server {

namespace playing {

SessionMazeMessageHandler::SessionMazeMessageHandler() {}
SessionMazeMessageHandler::~SessionMazeMessageHandler() {}

bool SessionMazeMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE,
      boost::bind(&SessionMazeMessageHandler::OnMessageMazeMapInfoSynchronize, this, _1, _2));

  return true;
}

void SessionMazeMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE);
}

void SessionMazeMessageHandler::OnMessageMazeMapInfoSynchronize(const char *data,
    size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageMazeMapInfoSynchronize sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageMazeMapInfoSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE from session_server.");

  MazeMapManager::GetInstance()->Update(sync);
  PlayingActorManager::GetInstance()->ResetMaze();
}

}  // namespace playing

}  // namespace server

}  // namespace game

