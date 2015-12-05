//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-16 23:08:19.
// File name: session_message_handler.cc
//
// Description:
//
#include "game_server/server/extension/mail/session_message_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/extension/mail/mail_pool.h"
#include "gateway_server/protocol/gateway_mail_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/types.h"
#include "global/common_functions.h"
#include "session_server/protocol/game_mail_protocol_types.h"

namespace game {

namespace server {

namespace mail {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  //MESSAGE_MAIL_NOTICE
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAIL_NOTICE,
      boost::bind(&SessionMessageHandler::OnMessageMailNotice, this, _1, _2));

  //MESSAGE_MAIL_SEND_RESPONSE
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_MAIL_SEND_RESPONSE,
      boost::bind(&SessionMessageHandler::OnMessageMailsendRequest, this, _1, _2));
  return true;
}

void SessionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
     session::protocol::GameMessageType::MESSAGE_MAIL_NOTICE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
     session::protocol::GameMessageType::MESSAGE_MAIL_SEND_RESPONSE);
}

void SessionMessageHandler::OnMessageMailNotice(
  const char *data, size_t size) {
  if(data == 0 || size == 0) {
    LOG_ERROR("解析协议参数错误");
    return ;
  }

  // 解析协议
  session::protocol::MessageMailNotice notice;
  if(global::GlobalPacket::GetPacket()->Deserialize(notice, 
        data, size) == false) {
    LOG_ERROR("解析消息 GameMessageMailNotice 失败");
    return ;
  }

  MailActor *mail_actor = MailActorManager::GetInstance()->Get(notice.addressee_id_);
  if(mail_actor == NULL) {
    LOG_ERROR("玩家邮件扩展不存在");
    return ;
  }

  GameActor *game_actor = mail_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩不存在[%lu]", notice.addressee_id_);
    return ;
  }
  Mail* p_mail = MailPool::GetInstance()->Allocate();
  if(p_mail == NULL) {
    LOG_ERROR("玩家(%lu) 邮件 模块内存分配失败");
    return ;
  }
  if(p_mail->Initialize(mail_actor) == false) { 
    LOG_ERROR("玩家(%lu) 邮件 初始化失败");
    MailPool::GetInstance()->Deallocate(p_mail);
    return ;
  }
  p_mail->SetDbId(notice.id_);
  p_mail->SetMailAffixType(notice.affix_type_);
  p_mail->SetMailCreateTime(notice.time_);
  p_mail->SetMailReadState(entity::MailReadState::UNREAD);
  p_mail->SetMailType(notice.type_);
  // 加载邮件到玩家身上去
  if(mail_actor->AddMail(p_mail) == false) {
    p_mail->Finalize();
    MailPool::GetInstance()->Deallocate(p_mail);
    //  LOG_ERROR("玩家(%lu) 邮件(%d)加载 失败", request.addressee_id_, p_mail->GetMailID());
    return ;
  }
  // 通知玩家新邮件到达
  gateway::protocol::MessageMailAdd add_mail;
  add_mail.mail_.__set_title_(notice.title_);
  add_mail.mail_.__set_addresser_(notice.addresser_);
  add_mail.mail_.__set_state_(notice.state_);
  add_mail.mail_.__set_affix_type_(notice.affix_type_);
  add_mail.mail_.__set_type_(notice.type_);
  add_mail.mail_.__set_time_(global::ToString(notice.time_));
  add_mail.mail_.__set_id_(notice.id_);

  game_actor->SendMessage(add_mail,
      gateway::protocol::MessageType::MESSAGE_MAIL_ADD);
}

void SessionMessageHandler::OnMessageMailsendRequest(
    const char *data, size_t size) {
  if(data == 0 || size == 0) {
    LOG_ERROR("解析协议参数错误");
    return ;
  }

/*  // 解析协议
  session::protocol::MessageMailSendResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(response, 
        data, size) == false) {
    LOG_ERROR("解析消息 GameMessageMailSendResponse 失败");
    return ;
  }

  MailActor *mail_actor = MailActorManager::GetInstance()->Get(response.addresser_id_);
  if(mail_actor == NULL) {
    LOG_ERROR("玩家邮件扩展不存在");
    return ;
  }

  GameActor *game_actor = mail_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩不存在[%lu]", response.addresser_id_);
    return ;
  }

  gateway::protocol::MessageMailSendResponse msg;
  msg.__set_result_(response.result_);
  game_actor->SendMessage(msg,
      gateway::protocol::MessageType::MESSAGE_MAIL_SEND_RESPONSE);
*/
}

} // namespace mail

} // namespace server

} // namespace game
