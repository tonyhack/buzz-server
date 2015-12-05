#include "game_server/server/extension/mail/mail_actor_manager.h"

#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_mail_add_types.h"
#include "game_server/server/extension/mail/mail_actor.h"

namespace game {

namespace server {

namespace mail {

MailActorManager::MailActorManager() {}
MailActorManager::~MailActorManager() {}

MailActor *MailActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool MailActorManager::Add(MailActor *actor) {
  // 检测指针是否合法
  if(actor == NULL) {
    return false;
  }
  // 检测是否存在
  ActorHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  // 加入管理器中
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

MailActor *MailActorManager::Remove(core::uint64 id) {
  MailActor *actor = NULL;
  ActorHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }
  return actor;
}

}  // namespace mail

}  // namespace server

}  // namespace game
