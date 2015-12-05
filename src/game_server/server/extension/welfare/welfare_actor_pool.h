
#ifndef __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__POOL__H
#define __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__POOL__H

#include "entity/achieve_types.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace welfare {

class WelfareActor;

class WelfareActorPool : public global::SingletonFactory<WelfareActorPool> {
  friend class global::SingletonFactory<WelfareActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  WelfareActor *Allocate();
  void Deallocate(WelfareActor *actor);

 private:
  WelfareActorPool() {}
  ~WelfareActorPool() {}

  global::PoolTemplate<WelfareActor> actors_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__ACHIEVE__ACTOR__POOL__H

