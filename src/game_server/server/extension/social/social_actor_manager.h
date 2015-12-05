#ifndef __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H
#define __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace social {

class SocialActor;

class SocialActorManager : public global::SingletonFactory<SocialActorManager> {
  friend class global::SingletonFactory<SocialActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SocialActor *> ActorHashmap;

 public:
  SocialActor *Get(core::uint64 id);
  bool Add(SocialActor *actor);
  SocialActor *Remove(core::uint64 id);

 private:
  SocialActorManager() {}
  ~SocialActorManager() {}

  ActorHashmap actors_;
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H

