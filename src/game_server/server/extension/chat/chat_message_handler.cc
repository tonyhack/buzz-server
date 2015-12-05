#include "game_server/server/extension/chat/chat_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "entity/chat_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_chat_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_chat_protocol_types.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/game_server.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/chat/configure.h"
#include "game_server/server/extension/chat/facade_request.h"
#include "game_server/server/extension/chat/chat_actor.h"
#include "game_server/server/extension/chat/chat_actor_manager.h"

namespace game {

namespace server {

namespace chat {

ChatMessageHandler::ChatMessageHandler() {}
ChatMessageHandler::~ChatMessageHandler() {}

bool ChatMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_CHAT_REQUEST,
      boost::bind(&ChatMessageHandler::OnMessageChatRequest, this, _1, _2, _3));

  return true;
}

void ChatMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_CHAT_REQUEST);
}

static void SendMessageChatResponse(ChatActor *chat_actor,
    gateway::protocol::ChatErrorCode::type error_code) {
  gateway::protocol::MessageChatResponse response;
  response.__set_result_(error_code);
  chat_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_CHAT_RESPONSE);
}

void ChatMessageHandler::OnMessageChatRequest(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 获取聊天玩家对象
  ChatActor *chat_actor = ChatActorManager::GetInstance()->Get(id);
  if (NULL == chat_actor) {
    LOG_ERROR("Get ChatActor(%lu) from ChatActorManager failed.", id);
    return;
  }
  GameActor *game_actor = chat_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageChatRequest request;
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize MessageChatRequest failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_CHAT_REQUEST from actor(%lu, %s), "
           "channel_=[%d], message_=[%s], target_name_=[%s]",
           game_actor->GetID(), game_actor->GetName().c_str(),
           request.channel_, request.message_.c_str(), request.target_name_.c_str());

  const ChatChannelCell *cell = Configure::GetInstance()->GetChatChannel(request.channel_);
  if (NULL == cell) {
    LOG_WARNING("Chat channel is invalid.");
    SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
    return;
  }

  // 检查长度
  ssize_t visual_length = global::GetStrVisualLengthUTF8(request.message_.c_str());
  if (visual_length < 0 || visual_length > cell->max_char_) {
    LOG_WARNING("Message is invalid.");
    SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
    return;
  }

  if (cell->cooling_ > 0) {
    // 检查CD
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    time_t last_chat_time = chat_actor->GetLastChatTime(request.channel_);

    if (last_chat_time + cell->cooling_ > now) {
      LOG_WARNING("Chat is in cooling.");
      SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }
    chat_actor->SetLastChatTime(request.channel_, now);
  }

  if (cell->spend_item_id_ != 0) {
    // 检查道具消耗
    if (FacadeRequest::GetInstance()->CheckPacketItemExist(id, cell->spend_item_id_,
            cell->spend_item_count_, BindType::BOTH) == false) {
      LOG_WARNING("Item(%u) is not enough.", cell->spend_item_id_);
      SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }

    // 扣除消耗
    core::uint32 delete_number = FacadeRequest::GetInstance()->RemovePacketItem(id,
        cell->spend_item_id_, cell->spend_item_count_, BindType::BOTH);
    if (delete_number != (core::uint32)cell->spend_item_count_) {
      LOG_ERROR("Remove item(%d) failed, req_count=[%d], del_count=[%d].",
                cell->spend_item_id_, cell->spend_item_count_, delete_number);
      SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }
  }

  // 过滤聊天信息
  GameServerSingleton::GetInstance().GetWordFilter()->Block(&request.message_);

  char string_id[32];
  snprintf(string_id, sizeof(string_id), "%lu", game_actor->GetID());

  gateway::protocol::MessageChatSynchronize sync;
  sync.__set_channel_(request.channel_);
  sync.__set_message_(request.message_);
  sync.__set_actor_id_(string_id);
  sync.__set_actor_name_(game_actor->GetName());

  if (entity::ChatChannelType::WORLD == request.channel_ ||
      entity::ChatChannelType::WORLD_VIP == request.channel_) {
    GameServerSingleton::GetInstance().BroadcastMessage(sync,
        gateway::protocol::MessageType::MESSAGE_CHAT_SYNCHRONIZE);
    SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::SUCCESS);
  } else if (entity::ChatChannelType::SCENE == request.channel_) {
    GameServerSingleton::GetInstance().BroadcastSceneMessage(sync,
        gateway::protocol::MessageType::MESSAGE_CHAT_SYNCHRONIZE, game_actor->GetScene());
    SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::SUCCESS);
  } else {
    if (request.__isset.target_name_ && request.target_name_ == game_actor->GetName()) {
      LOG_WARNING("Can not chat to yourself");
      SendMessageChatResponse(chat_actor, gateway::protocol::ChatErrorCode::ERROR_UNKNOWN);
      return;
    }

    // 转发 session_server
    session::protocol::MessageChatRequest session_request;
    session_request.__set_actor_id_(id);
    session_request.__set_channel_(request.channel_);
    session_request.__set_message_(request.message_);
    if (request.__isset.target_name_) {
      session_request.__set_target_name_(request.target_name_);
    }
    SessionChannel::SendMessage(session_request,
        session::protocol::GameMessageType::MESSAGE_CHAT_REQUEST);
  }
}

}  // namespace chat

}  // namespace server

}  // namespace game

