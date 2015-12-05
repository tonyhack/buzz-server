#ifndef __GAME__SERVER__ATTR__ATTR__ACTOR__MANAGER__H
#define __GAME__SERVER__ATTR__ATTR__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace attr {

class AttrActor;

class AttrActorManager : public global::SingletonFactory<AttrActorManager> {
  friend class global::SingletonFactory<AttrActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, AttrActor *> ActorHashmap;

 public:
  AttrActor *Get(core::uint64 id);
  bool Add(AttrActor *actor);
  AttrActor *Remove(core::uint64 id);

  void DailyClean();

 private:
  AttrActorManager() {}
  ~AttrActorManager() {}

  ActorHashmap actors_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ATTR__ACTOR__MANAGER__H

