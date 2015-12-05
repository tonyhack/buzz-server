#include "game_server/server/extension/mail/mail_pool.h"

#include "game_server/server/extension/mail/mail.h"

namespace game {

namespace server {

namespace mail {

MailPool::MailPool() {}
MailPool::~MailPool() {}

bool MailPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->mails_.Initialize(initial_number, extend_number);
}

void MailPool::Finalize() {
  this->mails_.Finalize();
}

Mail *MailPool::Allocate() {
  return this->mails_.Allocate();
}

void MailPool::Deallocate(Mail *p_mail) {
  if(p_mail == NULL)
    return ;
  mails_.Deallocate(p_mail);
}


} // namespace mail

} // namespace server

} // namespace game
