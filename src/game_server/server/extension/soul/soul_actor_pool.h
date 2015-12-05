//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-14 15:03:46.
// File name: soul_actor_pool.h
//
// Description:
// Define class SoulActorPool.
//

#ifndef __GAME__SERVER__SOUL__SOUL__ACTOR__POOL__H
#define __GAME__SERVER__SOUL__SOUL__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace soul {

class SoulActor;

class SoulActorPool : public global::SingletonFactory<SoulActorPool> {
  friend class global::SingletonFactory<SoulActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SoulActor *Allocate();
  void Deallocate(SoulActor *actor);

 private:
  SoulActorPool() {}
  ~SoulActorPool() {}

  global::PoolTemplate<SoulActor> actors_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ACTOR__POOL__H

