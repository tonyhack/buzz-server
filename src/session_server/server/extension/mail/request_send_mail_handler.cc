//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-30 03:41:34.
// File name: request_send_mail.cc
//
// Description:
//
#include "session_server/server/extension/mail/request_send_mail_handler.h"

#include <boost/bind.hpp>
#include "database_server/protocol/storage_mail_add_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/client/database_client.h"
#include "entity/mail_types.h"
#include "session_server/protocol/game_mail_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_terminal.h"
#include "session_server/server/request/session_mail_request_types.h"
#include "session_server/server/session_actor_manager.h"
#include "global/global_packet.h"
#include "global/common_functions.h"
#include "global/logging.h"

namespace session {

namespace server {

namespace mail {

RequestSendMailHandler::RequestSendMailHandler() {}
RequestSendMailHandler::~RequestSendMailHandler() {}

bool RequestSendMailHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_MAIL_SEND_MAIL,
      boost::bind(&RequestSendMailHandler::OnRequestSendMail, this, _1, _2));
  return true;
}

void RequestSendMailHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
     request::RequestType::REQUEST_MAIL_SEND_MAIL);
}

int RequestSendMailHandler::OnRequestSendMail(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSendMail) != size) {
    LOG_ERROR("请求增加邮件时，参数错误");
    return -1;
  }
  request::RequestSendMail *request = (request::RequestSendMail *)message;
  if(request->__isset.type_ == false) {
    request->type_  = entity::MailType::SYSTEM;
  }
  if(entity::MailType::SYSTEM != request->type_ &&
        entity::MailType::OPERATING != request->type_) {
     LOG_ERROR("邮件类型不正确[%d]", (core::int32)request->type_);
     return -1;
  }

  BriefActor *brief_actor = SessionActorManager::GetInstance()->GetBriefActor(request->id_);
  if(brief_actor == NULL) {
    LOG_ERROR("玩家不存在[%lu]", request->id_);
    return -1;
  }

  std::string params;
  params += global::ToString((core::uint64)request->child_type_);
  params += ",";
  for(core::uint32 i = 0; i < request->params_.size(); ++i) {
    params += request->params_[i];
    params += ",";
  }

  // 通知数据库
  database::protocol::StorageAddMailRequest send_msg;
  send_msg.__set_affix_(request->affix_);
  send_msg.__set_type_(request->type_);
  send_msg.__set_addresser_id_(0);
  send_msg.__set_addressee_id_(request->id_);
  send_msg.__set_context_(request->context_);
  send_msg.__set_title_(params);
  //send_msg.__set_addresser_();
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        send_msg, database::protocol::StorageType::STORAGE_MAIL_ADD, request->id_) == false) {
    LOG_ERROR("向数据库代理服务器请求失败");
    return -1;
  }
  return 0;
}

} // namespace mail

} // namespace server 

} // namespace session
