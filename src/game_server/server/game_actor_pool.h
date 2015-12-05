//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:11:46.
// File name: game_actor_pool.h
//
// Description:
// Define class GameActorPool.
//

#ifndef __GAME__SERVER__GAME__ACTOR__POOL__H
#define __GAME__SERVER__GAME__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameActor;

class GameActorPool : public global::SingletonFactory<GameActorPool> {
  friend class global::SingletonFactory<GameActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  GameActor *Allocate();
  void Deallocate(GameActor *actor);

 private:
  GameActorPool() {}
  ~GameActorPool() {}

  global::PoolTemplate<GameActor> actors_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__ACTOR__POOL__H

