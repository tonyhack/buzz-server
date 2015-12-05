#ifndef __GAME__SERVER__AUCTION__AUCTION__ACTOR__POOL__H
#define __GAME__SERVER__AUCTION__AUCTION__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/auction/auction_actor.h"

namespace game {

namespace server {

namespace auction {

class AuctionActorPool : public global::SingletonFactory<AuctionActorPool> {
  friend class global::SingletonFactory<AuctionActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  AuctionActor *Allocate();
  void Deallocate(AuctionActor *actor);

 private:
  AuctionActorPool() {}
  ~AuctionActorPool() {}

  global::PoolTemplate<AuctionActor> actors_;
};

}  // namespace auction

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AUCTION__AUCTION__ACTOR__POOL__H

