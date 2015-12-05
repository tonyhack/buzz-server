#include "session_server/server/extension/chat/game_chat_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "entity/chat_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_chat_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_chat_protocol_types.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"
#include "session_server/server/request/session_team_request_types.h"
#include "session_server/server/extension/chat/facade_request.h"

namespace session {

namespace server {

namespace chat {

GameChatMessageHandler::GameChatMessageHandler() {}
GameChatMessageHandler::~GameChatMessageHandler() {}

bool GameChatMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_CHAT_REQUEST,
      boost::bind(&GameChatMessageHandler::OnMessageChatRequest, this, _1, _2, _3));

  return true;
}

void GameChatMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_CHAT_REQUEST);
}

static void SendMessageChatResponse(SessionActor *session_actor,
    gateway::protocol::ChatErrorCode::type error_code) {
  gateway::protocol::MessageChatResponse response;
  response.__set_result_(error_code);
  session_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_CHAT_RESPONSE);
}

void GameChatMessageHandler::OnMessageChatRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if (NULL == terminal || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageChatRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageChatRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_CHAT_REQUEST from game_server, "
           "actor_id_=[%lu], channel_=[%d], message_=[%s], target_name_=[%s]",
           request.actor_id_, request.channel_,
           request.message_.c_str(), request.target_name_.c_str());

  // 获取发言玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(request.actor_id_);
  if (NULL == session_actor) {
    LOG_WARNING("Get SessionActor(%lu) from SessionActorManager failed.", request.actor_id_);
    return;
  }

  gateway::protocol::MessageChatSynchronize sync;
  sync.__set_channel_(request.channel_);
  sync.__set_message_(request.message_);
  sync.__set_actor_id_(session_actor->GetStringID());
  sync.__set_actor_name_(session_actor->GetName());

  if (entity::ChatChannelType::WHISPER == request.channel_) {
    // 检查私聊对象是否在线
    SessionActor *target_session_actor =
        SessionActorManager::GetInstance()->GetActor(request.target_name_);
    if (NULL == target_session_actor) {
      SendMessageChatResponse(session_actor,
          gateway::protocol::ChatErrorCode::TARGET_OFFLINE);
      return;
    }
    // 检查是否被对方拉黑
    if (FacadeRequest::GetInstance()->CheckInBlacklist(
            target_session_actor->GetActorID(), session_actor->GetActorID())) {
      SendMessageChatResponse(session_actor,
          gateway::protocol::ChatErrorCode::IN_TARGET_BLACKLIST);
      return;
    }

    // 转发聊天消息
    target_session_actor->SendMessage(sync,
        gateway::protocol::MessageType::MESSAGE_CHAT_SYNCHRONIZE);

    // 发送私聊请求回复
    gateway::protocol::MessageChatResponse response;
    response.__set_result_(gateway::protocol::ChatErrorCode::SUCCESS);
    response.__set_target_id_(target_session_actor->GetStringID());
    response.__set_target_name_(target_session_actor->GetName());
    response.__set_message_(request.message_);
    session_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_CHAT_RESPONSE);

  } else if (entity::ChatChannelType::TEAM == request.channel_) {
    // 获取小队成员列表
    request::RequestTeamGetMembers team_request;
    team_request.__set_actor_(session_actor->GetActorID());
    if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TEAM_GET_MEMBERS,
            &team_request, sizeof(team_request)) != 0) {
      LOG_WARNING("Get team members of Actor(%lu) failed.", session_actor->GetActorID());
      SendMessageChatResponse(session_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }

    // 转发聊天消息
    for (size_t i = 0; i < team_request.members_.size(); ++i) {
      SessionActor *team_session_actor =
          SessionActorManager::GetInstance()->GetActor(team_request.members_[i]); 
      if (NULL == team_session_actor) {
        continue;
      }
      team_session_actor->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_CHAT_SYNCHRONIZE);
    }

    // 发送成功消息
    SendMessageChatResponse(session_actor,
      gateway::protocol::ChatErrorCode::SUCCESS);

  } else if (entity::ChatChannelType::GUILD == request.channel_) {
    // 检查是否有在公会频道聊天的权限
    if (FacadeRequest::GetInstance()->CheckGuildAuthorityChat(
            session_actor->GetActorID()) == false)  {
      LOG_WARNING("No authority_chat.");
      SendMessageChatResponse(session_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }

    // 获取公会成员列表
    request::RequestGuildGetMembers guild_request;
    guild_request.__set_actor_id_(session_actor->GetActorID());
    if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_GUILD_GET_MEMBERS,
            &guild_request, sizeof(guild_request)) != 0) {
      LOG_WARNING("Get guild members of Actor(%lu) failed.", session_actor->GetActorID());
      SendMessageChatResponse(session_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }

    // 转发聊天消息
    for (size_t i = 0; i < guild_request.members_.size(); ++i) {
      SessionActor *guild_session_actor =
          SessionActorManager::GetInstance()->GetActor(guild_request.members_[i]);
      if (NULL == guild_session_actor) {
        continue;
      }
      guild_session_actor->SendMessage(sync,
          gateway::protocol::MessageType::MESSAGE_CHAT_SYNCHRONIZE);
    }

    // 发送成功消息
    SendMessageChatResponse(session_actor,
      gateway::protocol::ChatErrorCode::SUCCESS);
  }

}

}  // namespace chat

}  // namespace server

}  // namespace session 

