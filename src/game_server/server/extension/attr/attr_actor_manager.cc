#include "game_server/server/extension/attr/attr_actor_manager.h"

#include <cstddef>

#include "game_server/server/extension/attr/attr_actor.h"

namespace game {

namespace server {

namespace attr {

AttrActor *AttrActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool AttrActorManager::Add(AttrActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

AttrActor *AttrActorManager::Remove(core::uint64 id) {
  AttrActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

void AttrActorManager::DailyClean() {
  ActorHashmap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    AttrActor *attr_actor = iterator->second;
    if(NULL == attr_actor) {
      LOG_ERROR("AttrActor is null.");
      continue;
    }
    if (attr_actor->CheckLoadFinish() == false) {
      continue;
    }
    attr_actor->DailyClean();
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

