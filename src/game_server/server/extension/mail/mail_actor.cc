#include "game_server/server/extension/mail/mail_actor.h"

#include "game_server/server/extension/mail/mail_pool.h"
#include "game_server/server/extension/mail/mail.h"

namespace game {

namespace server {

namespace mail {

MailActor::MailActor() {}
MailActor::~MailActor() {}

bool MailActor::Initialize(GameActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("玩家邮件列表初始化失败");
    return false;
  }
  load_finish_ = false;
  game_actor_ = actor;
  return true;
}

void MailActor::Finalize() {
  MailMap::iterator iterator = mails_.begin();
  for(; iterator != mails_.end(); ++iterator) {
    Mail *mail = iterator->second;
    if(mail != NULL) {
      mail->Finalize();
      MailPool::GetInstance()->Deallocate(mail);
    }
  }
  mails_.clear();
  game_actor_ = NULL;
}

Mail *MailActor::GetMail(core::int32 id) {
  MailMap::iterator itr = mails_.find(id);
  if(itr != mails_.end())
    return itr->second;
  return NULL;
}

bool MailActor::EraseMail(core::int32 id) {
  MailMap::iterator itr = mails_.find(id);
  if(itr == mails_.end()) {
    return false;
  }
  itr->second->Finalize();
  MailPool::GetInstance()->Deallocate(itr->second);
  mails_.erase(itr);
  return true;
}

bool MailActor::AddMail(Mail* mail) {
  if(mail == NULL) {
    return false;
  }
  if(this->mails_.find(mail->GetMailID()) != this->mails_.end()) {
    LOG_ERROR("玩家(%lu) 邮件已存在邮件列表中", this->GetID());
    return false;
  }
  mail->StartExpireTimer();
  mails_.insert(MailMap::value_type(mail->GetMailID(), mail));
  return true;
}


}  // namespace mail

}  // namespace server

}  // namespace game
