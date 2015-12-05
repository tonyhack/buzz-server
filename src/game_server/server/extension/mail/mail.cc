#include "game_server/server/extension/mail/mail.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_del_types.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/mail/mail_actor.h"
#include "game_server/server/extension/mail/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace mail {

Mail::Mail() {}
Mail::~Mail() {}

bool Mail::Initialize(MailActor* actor) {
  if(actor == NULL) {
    LOG_ERROR("Invalid param actor is NULL");
    return false;
  }
  
  this->actor_ = actor;
  this->create_time_ = 0;
  this->id_ = 0;
  this->type_ = (core::int8)entity::MailType::INVALID;
  this->affix_type_ = (core::int8)entity::MailAffixType::NONE;
  this->read_state_ = (core::int8)entity::MailReadState::UNREAD;
  this->read_send_ = false;

  return true;
}

void Mail::Finalize() {
  GameServerSingleton::GetInstance().RemoveTimer(DISABLE_TIMER, this); 
  this->actor_ = NULL;
  this->create_time_ = 0;
  this->id_ = 0;
  this->type_ = (core::int8)entity::MailType::INVALID;
  this->affix_type_ = (core::int8)entity::MailAffixType::NONE;
  this->read_state_ = (core::int8)entity::MailReadState::UNREAD;
  this->read_send_ = false;
}

void Mail::StartExpireTimer() {
  // 先删除原来的定时器
  GameServerSingleton::GetInstance().RemoveTimer(DISABLE_TIMER, this); 

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  const  MailCell *cell = mail::Configure::GetInstance()->GetMailCell((
      entity::MailType::type)this->type_);
  time_t expiry = 0;
  if(cell == NULL) {
    LOG_ERROR("邮件配置文件有问题(%d)", this->GetMailType());
    return ;
  } else {
    expiry = cell->expiry_time_;
  }

  // 已过期的邮件
  if(this->create_time_ + expiry <= now ) {
    LOG_DEBUG("MailActor[%lu] mail(%d) expired!", this->actor_->GetID(), this->id_);
    DeleteMail();
    return ;
  }
  LOG_DEBUG("MailActor[%lu] mail start expire timer[%u] seconds.",
      this->actor_->GetID(), this->create_time_ + expiry - now);
  // 未过期
  GameServerSingleton::GetInstance().AddTimer(DISABLE_TIMER, (
      this->create_time_ + expiry - now) * 1000,
      1, this, "MailActor::StartExpireTimer");
}

void Mail::OnTimer(core::uint32 id) {
  if (id == DISABLE_TIMER) {
    // 邮件过期了,处理
    DeleteMail();
  }
}
/*
void Mail::CreateMail(entity::MailType::type mail_type, entity::MailAffixType::type mail_affix_type,
    core::int32 mail_ID, core::uint64 create_time) {
  SetDbId(mail_ID);
  SetMailAffixType(mail_affix_type);
  SetMailCreateTime(create_time);
  SetMailType(mail_type);
  }
  */
void Mail::DeleteMail() {
  if(actor_ == NULL ) {
    LOG_ERROR("没有找到玩家(%lu)", actor_->GetID());
    return ;
  }
  database::protocol::StorageDelMailRequest del_request;
  del_request.del_type_ = database::protocol::MailDelType::MAIL_DEL_TYPE_2;
  del_request.id_ = actor_->GetID();
  del_request.id_list_.push_back(this->id_);
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        del_request, database::protocol::StorageType::STORAGE_MAIL_DEL,
        actor_->GetID()) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  LOG_DEBUG("删除玩家(%lu)邮件(%d)", actor_->GetID(), this->id_);
}

}  // namespace mail

}  // namespace server 

}  // namespace game
