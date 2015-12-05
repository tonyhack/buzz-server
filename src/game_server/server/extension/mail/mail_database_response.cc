#include "game_server/server/extension/mail/mail_database_response.h"

#include <boost/bind.hpp>

#include "database_server/client/database_client.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/protocol/storage_gem_login_types.h"
#include "database_server/protocol/storage_mail_login_types.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "database_server/protocol/storage_mail_del_types.h"
#include "database_server/protocol/storage_mail_affix_types.h"
#include "database_server/protocol/storage_mail_read_types.h"
#include "database_server/protocol/storage_mail_affix_state_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/mail.h"
#include "game_server/server/extension/mail/mail_pool.h"
#include "game_server/server/extension/mail/mail_actor_pool.h"
#include "game_server/server/extension/mail/mail_actor_manager.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "gateway_server/protocol/gateway_mail_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/global_packet.h"
#include "entity/item_types.h"

namespace game {

namespace server {

namespace mail {

MailDatabaseResponse::MailDatabaseResponse() {}
MailDatabaseResponse::~MailDatabaseResponse() {}

bool MailDatabaseResponse::Initialize() {
  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_LOGIN,
      boost::bind(&MailDatabaseResponse::OnResponseGetAllMails, this, _1, _2, _3));

  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_DEL,
      boost::bind(&MailDatabaseResponse::OnResponseDelMail, this, _1, _2, _3));

  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_PICK_AFFIX,
      boost::bind(&MailDatabaseResponse::OnResponsePickAffx, this, _1, _2, _3));

  database::client::DatabaseClientSingleton::GetInstance().SetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_READ,
      boost::bind(&MailDatabaseResponse::OnResponseReadMail, this, _1, _2, _3));
  return true;
}

void MailDatabaseResponse::Finalize() {
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_LOGIN);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_DEL);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_PICK_AFFIX);
  database::client::DatabaseClientSingleton::GetInstance().ResetResponseCallback(
      database::protocol::StorageType::STORAGE_MAIL_READ);
}

void MailDatabaseResponse::OnResponseAddMail(const std::string &req, 
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageAddMailRequest request;
  database::protocol::StorageAddMailResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件发送失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.addresser_id_, result);
    return ;
  }

  if(request.type_ == entity::MailType::GENERAL) {
    // 从管理器中获取玩家对象
    GameActor *actor = GameActorManager::GetInstance()->GetActor(request.addresser_id_);
    if(actor == NULL) {
      LOG_ERROR("获取 GameActor(%lu) 失败", request.addresser_id_);
      return ;
    }

    gateway::protocol::MessageMailSendResponse msg;
    msg.result_ = response.mail_head_.id_ != 0;
    if(msg.result_ == false) {
      LOG_DEBUG("收件人没有找到(%d)", response.mail_head_.id_);
    }
    actor->SendMessage(msg,
        gateway::protocol::MessageType::MESSAGE_MAIL_SEND_RESPONSE);
  }

  // 通知另外的玩家
  GameActor* addressee_actor = GameActorManager::GetInstance()->GetActor(response.addressee_id_);
  if(addressee_actor == NULL) {
    // TODO: 不在本场景需要到session绕到目标玩家所在场景去处理
    LOG_ERROR("玩家(%lu)没有找到，在本场景中", response.addresser_id_);
  } else {
    gateway::protocol::MessageMailAdd add_mail;
    add_mail.mail_.__set_title_(request.title_);
    add_mail.mail_.__set_addresser_(response.mail_head_.addresser_);
    add_mail.mail_.__set_state_(response.mail_head_.state_);
    add_mail.mail_.__set_affix_type_(response.mail_head_.affix_type_);
    add_mail.mail_.__set_type_(response.mail_head_.type_);
    add_mail.mail_.__set_time_(global::ToString(response.mail_head_.time_));
    add_mail.mail_.__set_id_(response.mail_head_.id_);

    addressee_actor->SendMessage(add_mail, 
        gateway::protocol::MessageType::MESSAGE_MAIL_ADD);

    // 邮件扩展是否已存在
    MailActor *mail_actor = MailActorManager::GetInstance()->Get(request.addressee_id_);
    if(mail_actor == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
          __FILE__, __LINE__, __FUNCTION__, request.addressee_id_);
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
    p_mail->SetDbId(response.mail_head_.id_);
    p_mail->SetMailAffixType(response.mail_head_.affix_type_);
    p_mail->SetMailCreateTime(response.mail_head_.time_);
    p_mail->SetMailReadState(entity::MailReadState::UNREAD);
    p_mail->SetMailType(response.mail_head_.type_);
    // 加载邮件到玩家身上去
    if(mail_actor->AddMail(p_mail) == false) {
      p_mail->Finalize();
      MailPool::GetInstance()->Deallocate(p_mail);
      LOG_ERROR(" 玩家(%lu) 邮件(%d)加载 失败", request.addressee_id_, p_mail->GetMailID());
      return ;
    }
  }
}

void MailDatabaseResponse::OnResponseDelMail(
    const std::string &req, const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageDelMailRequest request;
  database::protocol::StorageDelMailResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("玩家(%lu) 删除邮件失败", request.id_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu) 获取失败", request.id_);
    return ;
  }

  // 邮件扩展是否已存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(request.id_);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }
  
  // 删除邮件
  for(size_t i = 0; i < request.id_list_.size(); ++i) {
    if(mail_actor->EraseMail(request.id_list_[i]) == false) {
       LOG_ERROR("(%lu)邮件没有删除成功(%d)", request.id_, request.id_list_[i]);
    }
  }
  // 区分请求删除邮件和主动删除邮件
  if(request.del_type_ == database::protocol::MailDelType::MAIL_DEL_TYPE_1) {
    gateway::protocol::MessageMailDelResponse message;
    message.__set_result_(true);
    actor->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_MAIL_DEL_RESPONSE);
  } else {
    gateway::protocol::MessageMailDel message;
    if(request.id_list_.size() == 1) {
      message.__set_id_(request.id_list_[0]);
      actor->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_MAIL_DEL);
    }
  }
}

void MailDatabaseResponse::OnResponseGetAllMails(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageMailLoginRequest request;
  database::protocol::StorageMailLoginResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("玩家(%lu)邮件初始化列表失败", request.id_);
    return ;
  }
  if(response.result_ != database::protocol::MailLoginResult::SUCCESS) {
    LOG_ERROR("玩家(%lu)邮件初始化列表失败", request.id_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu)对象失败", request.id_);
    return ;
  }

  // 邮件扩展是否已存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(request.id_);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 加载数据
  gateway::protocol::MessageMailInitialize message;
  gateway::protocol::MailContext message_mail;
  for (size_t i = 0 ; i < response.mail_list_.size(); ++i) {
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
    p_mail->SetDbId(response.mail_list_[i].id_);
    p_mail->SetMailAffixType(response.mail_list_[i].affix_type_);
    p_mail->SetMailCreateTime(response.mail_list_[i].time_);
    p_mail->SetMailReadState(response.mail_list_[i].state_);
    p_mail->SetMailType(response.mail_list_[i].type_);
    // 加载邮件到玩家身上去
    if(mail_actor->AddMail(p_mail) == false) {
      p_mail->Finalize();
      MailPool::GetInstance()->Deallocate(p_mail);
      LOG_ERROR(" 玩家(%lu) 邮件 （%d）加载 失败", request.id_, p_mail->GetMailID());
      continue;
    }
    message_mail.__set_addresser_(response.mail_list_[i].addresser_);
    message_mail.__set_affix_type_(response.mail_list_[i].affix_type_);
    message_mail.__set_id_(response.mail_list_[i].id_);
    message_mail.__set_state_(response.mail_list_[i].state_);
    message_mail.__set_time_(global::ToString(response.mail_list_[i].time_));
    message_mail.__set_title_(response.mail_list_[i].title_);
    message_mail.__set_type_(response.mail_list_[i].type_);
    message.mail_list_.push_back(message_mail);
  }
  actor->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_MAIL_INITIALIZE);

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(request.id_);
  load_finish.__set_extension_type_(ExtensionType::MAIL);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, request.id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) 邮件扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

void MailDatabaseResponse::OnResponsePickAffx(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StoragePickAffixRequest request;
  database::protocol::StoragePickAffixResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    LOG_ERROR("获取玩家(%lu)附件失败", request.id_);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu)对象失败", request.id_);
    return ;
  }

  // 邮件扩展是否已存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(request.id_);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }
  //查找邮件
  Mail *mail = mail_actor->GetMail(response.mail_id_);
  if(mail == NULL) {
    LOG_ERROR("玩家(%lu)邮件(%d)没有找到" , response.id_, response.mail_id_);
    return ;
  }

  request::RequestAddItems request_add_item;
  request_add_item.__set_actor_id_(response.id_);

  entity::AddItemFields field;
  field.__set_bind_(false);

  if(response.affix_.item1_ > 0 && response.affix_.item1_num_ > 0) {
    field.__set_item_template_(response.affix_.item1_);
    field.__set_number_(response.affix_.item1_num_);
    request_add_item.items_.push_back(field);
  }
  if(response.affix_.item2_ > 0 && response.affix_.item2_num_ > 0) {
    field.__set_item_template_(response.affix_.item2_);
    field.__set_number_(response.affix_.item2_num_);
    request_add_item.items_.push_back(field);
  }
  if(response.affix_.item3_ > 0 && response.affix_.item3_num_ > 0) {
    field.__set_item_template_(response.affix_.item3_);
    field.__set_number_(response.affix_.item3_num_);
    request_add_item.items_.push_back(field);
  }
  if(response.affix_.item4_ > 0 && response.affix_.item4_num_ > 0) {
    field.__set_item_template_(response.affix_.item4_);
    field.__set_number_(response.affix_.item4_num_);
    request_add_item.items_.push_back(field);
  }
  if(response.affix_.item5_ > 0 && response.affix_.item5_num_ > 0) {
    field.__set_item_template_(response.affix_.item5_);
    field.__set_number_(response.affix_.item5_num_);
    request_add_item.items_.push_back(field);
  }

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEMS,
        &request_add_item, sizeof(request_add_item)) != 0 ||
      request_add_item.result_ != request::AddItemResult::SUCCESS) {
    LOG_ERROR("玩家(%lu) 提取附件失败", actor->GetID());
    return ;
  }

  actor->AddResource(entity::ResourceID::GOLD, response.affix_.gold_);
  actor->AddResource(entity::ResourceID::RESTRICT_DOLLARS, response.affix_.restrict_dollars_);

  mail->SetMailAffixType(entity::MailAffixType::PICKED);

  // 通知玩家提取成功
  gateway::protocol::MessageMailPickAffixResponse send_message;
  send_message.__set_result_(true);
  actor->SendMessage(send_message,
      gateway::protocol::MessageType::MESSAGE_MAIL_PICK_AFFIX_RESPONSE);

  // 通知数据库附件提取完成
  database::protocol::StorageMailAffixStateRequest affix_request;
  affix_request.__set_id_(actor->GetID());
  affix_request.__set_mail_id_(response.mail_id_);
  affix_request.__set_type_(entity::MailAffixType::PICKED);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        affix_request, database::protocol::StorageType::STORAGE_MAIL_AFFIX_STATE,
        actor->GetID()) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
  }
}

void MailDatabaseResponse::OnResponseReadMail(const std::string &req,
    const std::string &res, database::protocol::ResponseType::type result) {
  // 消息解析
  database::protocol::StorageReadMailRequest request;
  database::protocol::StorageReadMailResponse response;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, req) == false ||
      global::GlobalPacket::GetPacket()->Deserialize(response, res) == false) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检测请求是否成功
  if(result != database::protocol::ResponseType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件阅读执行失败(%d)",
        __FILE__, __LINE__, __FUNCTION__, request.id_, result);
    return ;
  }

  // 从管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(request.id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu)，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  // 邮件扩展是否已存在
  MailActor *mail_actor = MailActorManager::GetInstance()->Get(request.id_);
  if(mail_actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 邮件扩展不存在",
        __FILE__, __LINE__, __FUNCTION__, request.id_);
    return ;
  }

  //设置邮件
  Mail *p_mail = mail_actor->GetMail(response.mail_id_);
  if(p_mail == NULL) {
    LOG_ERROR("玩家(%lu)邮件(%d)没有找到" , response.id_, response.mail_id_);
    return ;
  }
  p_mail->SetMailReadSend();
  p_mail->SetMailReadState(entity::MailReadState::READ);

  // 发送给玩家
  gateway::protocol::MessageMailReadResponse send_message;
  send_message.__set_affix_(response.affix_);
  send_message.__set_context_(response.context_);

  actor->SendMessage(send_message,
     gateway::protocol:: MessageType::MESSAGE_MAIL_READ_RESPONSE);
}

} // namespace mail

} // namespace server 

} // namespace game



