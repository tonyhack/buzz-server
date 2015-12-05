#include "game_server/server/extension/mail/mail_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "database_server/protocol/storage_mail_del_types.h"
#include "database_server/protocol/storage_mail_read_types.h"
#include "database_server/protocol/storage_mail_affix_types.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_mail_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/mail/configure.h"
#include "global/logging.h"
#include "global/types.h"
#include "session_server/protocol/game_mail_protocol_types.h"

namespace game {

namespace server {

namespace mail {

MailMessageHandler::MailMessageHandler() {}
MailMessageHandler::~MailMessageHandler() {}

bool MailMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_DEL_REQUEST,
      boost::bind(&MailMessageHandler::OnMessageEraseMail, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_PICK_AFFIX_REQUEST,
      boost::bind(&MailMessageHandler::OnMessagePickAffix, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_READ_REQUEST,
      boost::bind(&MailMessageHandler::OnMessageReadMail, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_SEND_REQUEST,
      boost::bind(&MailMessageHandler::OnMessageSendMail, this, _1, _2, _3));

  return true;
}

void MailMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_DEL_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_PICK_AFFIX_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_READ_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_MAIL_SEND_REQUEST);
}

void MailMessageHandler::OnMessageSendMail(core::uint64 id, const char *data, size_t size) {
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("玩家 (%lu) 没有找到", id);
    return ;
  }

  // 邮件玩家扩展是否存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(id);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageMailSendRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageMailSendRequest  失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_MAIL_SEND_REQUEST from actor(%lu, %s), addressee_(%s)",
      actor->GetActorID(), actor->GetName().c_str(), message.addressee_.c_str(),
      message.title_.c_str());

  // 验证玩家
  const MailCell *cell = Configure::GetInstance()->GetMailCell(entity::MailType::GENERAL);
  if(cell == NULL) {
    LOG_ERROR("配置文件出错(%d)",(core::int32)entity::MailType::GENERAL);
    return ;
  }
  
  if(cell->context_max_ < (core::int32)message.context_.size()) {
    LOG_ERROR("玩家发送的邮件正文超出(%d)", message.context_.size());
    return ;
  }
  core::uint64 addressee_id = 0;
  GameActor *addressee = GameActorManager::GetInstance()->GetActor(message.addressee_);
  if(addressee != NULL) {
    addressee_id = addressee->GetID();
  }
  
  session::protocol::MessageMailSendRequest send;
  send.__set_addressee_(message.addressee_);
  send.__set_addresser_(actor->GetName());
  send.__set_addressee_id_(addressee_id);
  send.__set_addresser_id_(actor->GetID());
  send.__set_title_(message.title_);
  send.__set_context_(message.context_);
  send.__set_type_(entity::MailType::GENERAL);
  send.__set_affix_type_(entity::MailAffixType::NONE);
  SessionChannel::SendMessage(send, 
      session::protocol::GameMessageType::MESSAGE_MAIL_SEND_REQUEST);
}

void MailMessageHandler::OnMessageEraseMail(core::uint64 id, const char *data, size_t size) {
  // 邮件玩家扩展是否存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(id);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("玩家 (%lu) 没有找到", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageMailDelRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageMailDelRequest  失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_MAIL_DEL_REQUEST from actor(%lu, %s), del_mail_list_ size(%lu)",
      actor->GetActorID(), actor->GetName().c_str(), message.del_mail_list_.size());

  for (size_t i = 0; i < message.del_mail_list_.size(); ++i) {
    if(mail_actor->GetMail(message.del_mail_list_[i]) == NULL) {
      LOG_ERROR("玩家（%lu）删除的邮件 (%d) 不存在", id, message.del_mail_list_[i]);
      return ;
    }
  }

  // 向数据库请求删除邮件数据
  database::protocol::StorageDelMailRequest send_message;
  send_message.__set_id_(id);
  send_message.__set_del_type_(database::protocol::MailDelType::MAIL_DEL_TYPE_1); 
  send_message.__set_id_list_(message.del_mail_list_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        send_message, database::protocol::StorageType::STORAGE_MAIL_DEL,
        id) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void MailMessageHandler::OnMessagePickAffix(core::uint64 id, const char *data, size_t size) {
  // 邮件玩家扩展是否存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(id);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("玩家 (%lu) 没有找到", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageMailPickAffixRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageMailPickAffixResponse  失败");
    return ;
  }

  // 验证邮件是否已经提取附件
  Mail *p_mail = mail_actor->GetMail(message.mail_id_);
  if(p_mail == NULL) {
    LOG_ERROR("玩家 (%lu) 的邮件 (%d) 没有找到", id, message.mail_id_);
    return ;
  }
  if(p_mail->GetMailAffixType() != entity::MailAffixType::EXIST) {
    LOG_ERROR("玩家(%lu)的邮件(%d)没有附件", id, message.mail_id_);
    return ;
  }
  database::protocol::StoragePickAffixRequest send_message;
  send_message.__set_id_(id);
  send_message.__set_mail_id_(message.mail_id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        send_message, database::protocol::StorageType::STORAGE_MAIL_PICK_AFFIX,
        id) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

void MailMessageHandler::OnMessageReadMail(core::uint64 id, const char *data, size_t size) {
  // 邮件玩家扩展是否存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(id);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    LOG_ERROR("玩家 (%lu) 没有找到", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageMailReadRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 OnMessageReadMail  失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_MAIL_READ_REQUEST from actor(%lu, %s), mail_id_(%lu)",
      actor->GetActorID(), actor->GetName().c_str(), message.mail_id_);

  // 验证邮件是否是读取的
  Mail *p_mail = mail_actor->GetMail(message.mail_id_);
  if(p_mail == NULL) {
    LOG_ERROR("玩家 (%lu) 的邮件 (%d) 没有找到", id, message.mail_id_);
    return ;
  }
  if(p_mail->GetMailReadSend() == true) {
    LOG_ERROR("玩家(%lu) 的邮件(%d)已经读取", id, message.mail_id_);
    return ;
  }

  // 向数据库发送请求消息
  database::protocol::StorageReadMailRequest send_message;
  send_message.__set_id_(id);
  send_message.__set_mail_id_(message.mail_id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        send_message, database::protocol::StorageType::STORAGE_MAIL_READ,
        id) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
}

} // namespace mail

} // namepsace server 

} // namespace game
