#include "game_server/server/extension/auction/auction_actor_pool.h"

namespace game {

namespace server {

namespace auction {

bool AuctionActorPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->actors_.Initialize(initial_number, extend_number);
}

void AuctionActorPool::Finalize() {
  this->actors_.Finalize();
}

AuctionActor *AuctionActorPool::Allocate() {
  return this->actors_.Allocate();
}

void AuctionActorPool::Deallocate(AuctionActor *actor) {
  this->actors_.Deallocate(actor);
}

}  // namespace auction

}  // namespace server

}  // namespace game

