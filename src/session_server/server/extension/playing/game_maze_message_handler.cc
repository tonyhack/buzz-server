#include "session_server/server/extension/playing/game_maze_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/extension/playing/maze_map_manager.h"

namespace session {

namespace server {

namespace playing {

GameMazeMessageHandler::GameMazeMessageHandler() {}
GameMazeMessageHandler::~GameMazeMessageHandler() {}

bool GameMazeMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_REQUEST,
      boost::bind(&GameMazeMessageHandler::OnMessageMazeMapInfoRequest, this, _1, _2, _3));

  return true;
}

void GameMazeMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_REQUEST);
}

void GameMazeMessageHandler::OnMessageMazeMapInfoRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageMazeMapInfoRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageMazeMapInfoRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_MAZE_MAP_INFO_REQUEST from game_server.");

  session::protocol::MessageMazeMapInfoSynchronize sync;
  MazeMapManager::GetInstance()->ExportMapInfo(sync);
  terminal->SendGameMessage(sync,
      session::protocol::GameMessageType::MESSAGE_MAZE_MAP_INFO_SYNCHRONIZE);
}

}  // namespace playing

}  // namespace server

}  // namespace session

