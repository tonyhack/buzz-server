#ifndef __GAME__SERVER__AUCTION__AUCTION__ACTOR__MANAGER__H
#define __GAME__SERVER__AUCTION__AUCTION__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace auction {

class AuctionActor;

class AuctionActorManager : public global::SingletonFactory<AuctionActorManager> {
  friend class global::SingletonFactory<AuctionActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, AuctionActor *> ActorHashmap;

 public:
  AuctionActor *Get(core::uint64 id);
  bool Add(AuctionActor *actor);
  AuctionActor *Remove(core::uint64 id);

 private:
  AuctionActorManager() {}
  ~AuctionActorManager() {}

  ActorHashmap actors_;
};

}  // namespace auction

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AUCTION__AUCTION__ACTOR__MANAGER__H

