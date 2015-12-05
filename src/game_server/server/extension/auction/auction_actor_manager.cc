#include "game_server/server/extension/auction/auction_actor_manager.h"

#include <cstddef>

#include "game_server/server/extension/auction/auction_actor.h"

namespace game {

namespace server {

namespace auction {

AuctionActor *AuctionActorManager::Get(core::uint64 id) {
  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    return iter->second;
  }

  return NULL;
}

bool AuctionActorManager::Add(AuctionActor *actor) {
  if (NULL == actor) {
    return false;
  }
  if (this->actors_.find(actor->GetID()) != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

AuctionActor *AuctionActorManager::Remove(core::uint64 id) {
  AuctionActor *actor = NULL;

  ActorHashmap::iterator iter = this->actors_.find(id);
  if (iter != this->actors_.end()) {
    actor = iter->second;
    this->actors_.erase(iter);
  }

  return actor;
}

}  // namespace auction

}  // namespace server

}  // namespace game

