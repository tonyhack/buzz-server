#include "session_server/server/extension/gm/game_gm_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_gm_protocol_types.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"

namespace session {

namespace server {

namespace gm {

GameGmMessageHandler::GameGmMessageHandler() {}
GameGmMessageHandler::~GameGmMessageHandler() {}

bool GameGmMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GM_COMMAND,
      boost::bind(&GameGmMessageHandler::OnMessageGmCommand, this, _1, _2, _3));

  return true;
}

void GameGmMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GM_COMMAND);
}

static void GmAddGuildAttribute(SessionActor *session_actor, request::GuildAttributeType::type attr_type,
    std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return;
  }

  core::int32 value = atoi(command_args[1].c_str());
  if (value != 0) {
    request::RequestGuildAddAttribute request;
    request.__set_actor_id_(session_actor->GetActorID());
    request.__set_type_(attr_type);
    request.__set_value_(value);
    ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GUILD_ADD_ATTRIBUTE,
        &request, sizeof(request));
  }
}

void GameGmMessageHandler::OnMessageGmCommand(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGmCommand message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageGmCommand failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GM_COMMAND from game_server, "
           "actor_id_=[%lu], command_=[%s]",
           message.actor_id_, message.command_.c_str());

  // 得到玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(message.actor_id_);
  if (NULL == session_actor) {
    LOG_WARNING("Get SessionActor(%lu) from SessionActorManager failed.", message.actor_id_);
    return;
  }

  // 命令解析
  std::vector<std::string> command_args;
  global::StringSplit(message.command_.c_str(), " ", command_args);

  if ("add_guild_light_attr" == command_args[0]) {
    // 增加公会光属性
    // usage: add_guild_light_attr <guild_light_attr>
    GmAddGuildAttribute(session_actor, request::GuildAttributeType::LIGHT, command_args);
  } else if ("add_guild_dark_attr" == command_args[0]) {
    // 增加公会暗属性
    // usage: add_guild_dark_attr <guild_dark_attr>
    GmAddGuildAttribute(session_actor, request::GuildAttributeType::DARK, command_args);
  } else if ("add_guild_holy_attr" == command_args[0]) {
    // 增加公会圣属性
    // usage: add_guild_holy_attr <guild_holy_attr>
    GmAddGuildAttribute(session_actor, request::GuildAttributeType::HOLY, command_args);
  } else {
    LOG_WARNING("Invalid command.");
  }
}

}  // namespace gm

}  // namespace server

}  // namespace session

