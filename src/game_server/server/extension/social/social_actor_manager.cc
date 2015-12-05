#include "game_server/server/extension/social/social_actor_manager.h"

#include <cstddef>

#include "game_server/server/extension/social/social_actor.h"

namespace game {

namespace server {

namespace social {

SocialActor *SocialActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool SocialActorManager::Add(SocialActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

SocialActor *SocialActorManager::Remove(core::uint64 id) {
  SocialActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

}  // namespace social

}  // namespace server

}  // namespace game

