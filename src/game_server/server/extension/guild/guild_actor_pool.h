#ifndef __GAME__SERVER__GUILD__GUILD__ACTOR__POOL__H
#define __GAME__SERVER__GUILD__GUILD__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/guild/guild_actor.h"

namespace game {

namespace server {

namespace guild {

class GuildActorPool : public global::SingletonFactory<GuildActorPool> {
  friend class global::SingletonFactory<GuildActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  GuildActor *Allocate();
  void Deallocate(GuildActor *actor);

 private:
  GuildActorPool() {}
  ~GuildActorPool() {}

  global::PoolTemplate<GuildActor> actors_;
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__ACTOR__POOL__H

