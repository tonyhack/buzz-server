#include "game_server/server/extension/chat/chat_actor_manager.h"

#include <cstddef>

#include "game_server/server/extension/chat/chat_actor.h"

namespace game {

namespace server {

namespace chat {

ChatActor *ChatActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool ChatActorManager::Add(ChatActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

ChatActor *ChatActorManager::Remove(core::uint64 id) {
  ChatActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

}  // namespace chat

}  // namespace server

}  // namespace game

