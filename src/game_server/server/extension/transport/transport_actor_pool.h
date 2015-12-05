//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 02:27:18.
// File name: transport_actor_pool.h
//
// Description:
// Define class TransportActorPool.
//

#ifndef __GAME__SERVER__TRANSPORT__ACTOR__POOL__H
#define __GAME__SERVER__TRANSPORT__ACTOR__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class TransportActor;

class TransportActorPool : public global::SingletonFactory<TransportActorPool> {
  friend class global::SingletonFactory<TransportActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  TransportActor *Allocate();
  void Deallocate(TransportActor *actor);

 private:
  TransportActorPool() {}
  ~TransportActorPool() {}

  global::PoolTemplate<TransportActor> actors_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__ACTOR__POOL__H

