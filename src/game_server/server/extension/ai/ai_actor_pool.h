//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-26 20:27:43.
// File name: ai_actor_pool.h
//
// Description:
//
#ifndef __GAME__SERVER__AI__AI__ACTOR__POOL__H
#define __GAME__SERVER__AI__AI__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace ai {

class AiActor;

class AiActorPool : public global::SingletonFactory<AiActorPool> {
  friend class global::SingletonFactory<AiActorPool>;
 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  AiActor *Allocate();
  void Deallocate(AiActor *actor);

 private:
  AiActorPool();
  ~AiActorPool();

  global::PoolTemplate<AiActor> ai_actors_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__AI__ACTOR__POOL__H

