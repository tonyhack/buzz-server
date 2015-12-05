//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-13 20:17:26.
// File name: game_message_handler.cc
//
// Description:
//
#include "session_server/server/extension/mail/game_message_handler.h"

#include <boost/bind.hpp>
#include "entity/constants_constants.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "gateway_server/protocol/gateway_mail_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_terminal.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_mail_protocol_types.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/error_code.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace mail {

GameMessageHandler::GameMessageHandler() {}
GameMessageHandler::~GameMessageHandler(){}

bool GameMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(session::protocol::GameMessageType::MESSAGE_MAIL_SEND_REQUEST,
      boost::bind(&GameMessageHandler::OnMessageMailSendRequest, this, _1, _2, _3));
  return true;
}

void GameMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAIL_SEND_REQUEST);
}

void GameMessageHandler::OnMessageMailSendRequest(SessionTerminal *terminal,
    const char *data, size_t size) {
  if(terminal == NULL || data == NULL || 0 == size) {
    LOG_ERROR("Invalid params.");
  }

  // 解析协议
  session::protocol::MessageMailSendRequest request;  
  if (global::GlobalPacket::GetPacket()->Deserialize(request, data, size) == false) {
    LOG_ERROR("Deserialize GameMessageMailSendRequest failed.");
    return ;
  }
  // 通知客户端
  SessionActor *addresser_actor = SessionActorManager::GetInstance()->GetActor(request.addresser_id_);
  gateway::protocol::MessageMailSendResponse notice_addresser;
  notice_addresser.__set_result_(false);

  if(entity::MailType::GENERAL == request.type_) {
    if(request.addressee_id_ == 0) {
      BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(request.addressee_);
      if(brief_actor == NULL) {
        LOG_ERROR("收件人没有找到 (%s)", request.addressee_.c_str());
        if(addresser_actor != NULL) {
          ErrorCode::Send(addresser_actor, entity::ErrorType::MAIL_ACTOR_INVALID);
          addresser_actor->SendMessage(notice_addresser,
              gateway::protocol::MessageType::MESSAGE_MAIL_SEND_RESPONSE);
        }
        return ;
      }
      request.__set_addressee_id_(brief_actor->GetID());
    }
  }

  database::protocol::StorageAddMailRequest message;
  if(request.type_ != entity::MailType::GENERAL) {
    char title_buff[entity::g_constants_constants.kMaxMailTitleLen];
    sprintf(title_buff,"%d", request.child_type_);
    int len = 0 ;
    for(core::uint32 i = 0; i < request.params_.size(); ++i) {
      len = strlen(title_buff);
      if((core::int32)(len + request.params_[i].size()) >= (core::int32)entity::g_constants_constants.kMaxMailTitleLen) {
        LOG_ERROR("邮件标题超出返回(%d)", len + request.params_[i].size());
        return ;
      }
      sprintf(title_buff + len, ",%s", request.params_[i].c_str());
    }
    message.__set_title_(title_buff);
  } else {
    message.__set_title_(request.title_);
    message.__set_context_(request.context_);
  }
  // 通知数据库
  message.__set_affix_(request.affix_);
  message.__set_type_(request.type_);
  message.__set_addresser_id_(request.addresser_id_);
  message.__set_addressee_id_(request.addressee_id_);
  message.__set_addresser_(request.addresser_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        message, database::protocol::StorageType::STORAGE_MAIL_ADD, request.addressee_id_) == false) {
    LOG_ERROR("向数据库代理服务器请求失败");
    return ;
  }
}

} // namespace mail

} // namespace server

} // namespace session

