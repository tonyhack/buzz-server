#include "session_server/server/extension/item/game_message_handler.h"

#include <boost/bind.hpp>
#include "entity/constants_constants.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_terminal.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_item_protocol_types.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/error_code.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace item {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler(){}

bool GameMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(session::protocol::GameMessageType::MESSAGE_ITEM_TREASURE_HUNT_SYNCHRONIZE,
      boost::bind(&GameMessageHandler::OnTreasureHuntSynchronize, this, _1, _2, _3));
  return true;
}

void GameMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ITEM_TREASURE_HUNT_SYNCHRONIZE);
}

void GameMessageHandler::OnTreasureHuntSynchronize(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || 0 == size) {
    LOG_ERROR("Invalid params.");
  }

  // 解析协议
  session::protocol::MessageItemTreasureHuntSynchronize request;  
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize GameMessageItemSendRequest failed.");
    return ;
  }

  for(size_t i = 0; i < request.info_.size(); ++i) {
    ItemExtension::GetInstance()->AddTreasureInfo(request.info_[i]);
  }
}

} // namespace item

} // namespace server

} // namespace session

