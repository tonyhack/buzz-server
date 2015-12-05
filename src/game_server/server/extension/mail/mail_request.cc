//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-08-30 21:59:43.
// File name: mail_request.cc
//
// Description:
//

#include "game_server/server/extension/mail/mail_request.h"

#include <boost/bind.hpp>

#include "entity/mail_types.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/mail/configure.h"
#include "game_server/server/extension/mail/mail.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/request/game_mail_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/session_channel.h"
#include "global/logging.h"
#include "session_server/protocol/game_mail_protocol_types.h"

namespace game {

namespace server {

namespace mail {

RequestMailHandler::RequestMailHandler() {}
RequestMailHandler::~RequestMailHandler() {}

bool RequestMailHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_MAIL_SEND,
      boost::bind(&RequestMailHandler::OnRequestMailSend, this, _1, _2));
  return true;
}

void RequestMailHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_MAIL_SEND);
}

int RequestMailHandler::OnRequestMailSend(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestMailSend) != size) {
    LOG_ERROR("请求增加邮件时，参数错误");
    return -1;
  }

  request::RequestMailSend *request = (request::RequestMailSend *)message;
  const  MailCell *cell = mail::Configure::GetInstance()->GetMailCell((
      entity::MailType::type)request->type_);
  if(cell == NULL) {
    LOG_ERROR("邮件发送失败，邮件类型或配置出错, mail type[%d]", 
        request->type_);
    return -1;
  }

  session::protocol::MessageMailSendRequest send;
  send.__set_addressee_id_(request->id_);
  send.__set_type_(request->type_);
  send.__set_affix_(request->affix_);
  if(request->affix_.item1_ != 0 && request->affix_.item1_num_) {
    send.__set_affix_type_(entity::MailAffixType::EXIST);
  } else if (request->affix_.item2_ != 0 && request->affix_.item2_num_) {
    send.__set_affix_type_(entity::MailAffixType::EXIST);
  } else if (request->affix_.item3_ != 0 && request->affix_.item3_num_) {
    send.__set_affix_type_(entity::MailAffixType::EXIST);
  } else if(request->affix_.item4_ != 0 && request->affix_.item4_num_) {
    send.__set_affix_type_(entity::MailAffixType::EXIST);
  } else if(request->affix_.item5_ != 0 && request->affix_.item5_num_) {
    send.__set_affix_type_(entity::MailAffixType::EXIST);
  }
  send.__set_params_(request->params_);
  send.__set_child_type_(request->child_type_);

  SessionChannel::SendMessage(send, 
      session::protocol::GameMessageType::MESSAGE_MAIL_SEND_REQUEST);

  return 0;
}

} // namespace mail

} // namespace server 

} // namespace game
