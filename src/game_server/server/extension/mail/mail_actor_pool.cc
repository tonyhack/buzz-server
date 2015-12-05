#include "game_server/server/extension/mail/mail_actor_pool.h"

namespace game {

namespace server {

namespace mail {

MailActorPool::MailActorPool() {}
MailActorPool::~MailActorPool() {}

bool MailActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->mail_actors_.Initialize(initial_number, extend_number);
}

void MailActorPool::Finalize() {
  this->mail_actors_.Finalize();
}

MailActor *MailActorPool::Allocate() {
  return this->mail_actors_.Allocate();
}

void MailActorPool::Deallocate(MailActor *mail_actor) {
  if(mail_actor == NULL)
    return ;
  mail_actors_.Deallocate(mail_actor);
}

}  // namespace mail
 
}  // namespace server 

}  // namespace game

