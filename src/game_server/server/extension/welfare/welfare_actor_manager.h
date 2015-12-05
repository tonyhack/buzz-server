
#ifndef __GAME__SERVER__WELFARE__WELFARE__ACTOR__MANAGER__H
#define __GAME__SERVER__WELFARE__WELFARE__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace welfare {

class WelfareActor;

class WelfareActorManager : public global::SingletonFactory<WelfareActorManager> {
  friend class global::SingletonFactory<WelfareActorManager>;

  typedef __gnu_cxx::hash_map<core::uint64, WelfareActor *> ActorHashmap;

 public:
  WelfareActor *Get(core::uint64 id);
  bool Add(WelfareActor *actor);
  WelfareActor *Remove(core::uint64 id);
  
  void DailyClean(core::int32 days);
 private:
  WelfareActorManager() {}
  ~WelfareActorManager() {}

  ActorHashmap actors_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__WELFARE__ACTOR__MANAGER__H

