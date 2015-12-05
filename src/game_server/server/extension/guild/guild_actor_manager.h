#ifndef __GAME__SERVER__GUILD__GUILD__ACTOR__MANAGER__H
#define __GAME__SERVER__GUILD__GUILD__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace guild {

class GuildActor;

class GuildActorManager : public global::SingletonFactory<GuildActorManager> {
  friend class global::SingletonFactory<GuildActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, GuildActor *> ActorHashmap;

 public:
  GuildActor *Get(core::uint64 id);
  bool Add(GuildActor *actor);
  GuildActor *Remove(core::uint64 id);

 private:
  GuildActorManager() {}
  ~GuildActorManager() {}

  ActorHashmap actors_;
};

}  // namespace guild

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GUILD__GUILD__ACTOR__MANAGER__H

