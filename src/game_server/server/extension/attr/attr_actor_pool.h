#ifndef __GAME__SERVER__ATTR__ATTR__ACTOR__POOL__H
#define __GAME__SERVER__ATTR__ATTR__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/attr/attr_actor.h"

namespace game {

namespace server {

namespace attr {

class AttrActorPool : public global::SingletonFactory<AttrActorPool> {
  friend class global::SingletonFactory<AttrActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  AttrActor *Allocate();
  void Deallocate(AttrActor *actor);

 private:
  AttrActorPool() {}
  ~AttrActorPool() {}

  global::PoolTemplate<AttrActor> actors_;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__ATTR__ACTOR__POOL__H

