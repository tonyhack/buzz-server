#include "game_server/server/extension/guild/guild_actor_manager.h"

#include <cstddef>

#include "game_server/server/extension/guild/guild_actor.h"

namespace game {

namespace server {

namespace guild {

GuildActor *GuildActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool GuildActorManager::Add(GuildActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

GuildActor *GuildActorManager::Remove(core::uint64 id) {
  GuildActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

}  // namespace guild

}  // namespace server

}  // namespace game

