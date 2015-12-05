//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-11-03 22:13:29.
// File name: bar_actor_pool.h
//
// Description:
//
#ifndef __GAME__SERVER__BAR__BAR__ACTOR__POOL__H
#define __GAME__SERVER__BAR__BAR__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace bar {

class BarActor;

class BarActorPool : public global::SingletonFactory<BarActorPool> {
  friend class global::SingletonFactory<BarActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  BarActor *Allocate();
  void Deallocate(BarActor *bar);
  
 private:
  BarActorPool() {}
  ~BarActorPool() {}

  global::PoolTemplate<BarActor> bars_;
};

}  // namespace bar

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BAR__BAR__ACTOR__POOL__H

