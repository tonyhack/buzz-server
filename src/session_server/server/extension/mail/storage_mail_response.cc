//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-13 20:02:49.
// File name: database_mail_response.cc
//
// Description:
//
#include "session_server/server/extension/mail/storage_mail_response.h"

#include <boost/bind.hpp>
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "gateway_server/protocol/gateway_mail_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/protocol/game_mail_protocol_types.h"

namespace session {

namespace server {

namespace mail {

StorageMailResponse::StorageMailResponse() {}
StorageMailResponse::~StorageMailResponse() {}

bool StorageMailResponse::Initialize() {
  // STORAGE_MAIL_ADD
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_ADD, boost::bind(
        &StorageMailResponse::response, this, _1, _2, _3));
  return true;
}

void StorageMailResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_ADD);
}

void StorageMailResponse::response(const std::string &req, const std::string &res,
    database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageAddMailRequest request;
  database::protocol::StorageAddMailResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {    
    LOG_ERROR("Deserialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("StorageAddMailRequest %d 失败", result);
    return ;
  }

  // 通知发件人
  SessionActor *addresser_actor = SessionActorManager::GetInstance()->GetActor(request.addresser_);
  if(addresser_actor != NULL) {
    gateway::protocol::MessageMailSendResponse notice_addresser;
    notice_addresser.__set_result_(true);
    addresser_actor->SendMessage(notice_addresser,
        gateway::protocol::MessageType::MESSAGE_MAIL_SEND_RESPONSE);
  } else {
    LOG_ERROR("发件人不在线 (%s)", request.addresser_.c_str());
  }

  // 通知收件人
  SessionActor *addressee_actor = SessionActorManager::GetInstance()->GetActor(request.addressee_id_);
  if(addressee_actor != NULL) {
    session::protocol::MessageMailNotice notice_addressee;
    notice_addressee.__set_title_(response.mail_head_.title_);
    notice_addressee.__set_addresser_(response.mail_head_.addresser_);
    notice_addressee.__set_state_(entity::MailReadState::UNREAD);
    notice_addressee.__set_affix_type_(response.mail_head_.affix_type_);
    notice_addressee.__set_time_(response.mail_head_.time_);
    notice_addressee.__set_id_(response.mail_head_.id_);
    notice_addressee.__set_addressee_id_(addressee_actor->GetActorID());
    notice_addressee.__set_type_(request.type_);
    addressee_actor->SendGameMessage(notice_addressee,
        session::protocol::GameMessageType::MESSAGE_MAIL_NOTICE);
  } else {
    LOG_ERROR("收件人不在线 (%lu)", request.addressee_id_);
  }
}

} // namespace mail

} // namespace server

} // namespace session

