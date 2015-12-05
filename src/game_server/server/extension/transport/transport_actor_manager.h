//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-08 21:35:48.
// File name: transport_actor_manager.h
//
// Description:
// Define class TransportActorManager.
//

#ifndef __GAME__SERVER__TRANSPORT__ACTOR__MANAGER__H
#define __GAME__SERVER__TRANSPORT__ACTOR__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class TransportActor;

class TransportActorManager : public global::SingletonFactory<TransportActorManager> {
  friend class global::SingletonFactory<TransportActorManager>;
  typedef __gnu_cxx::hash_map<core::uint64, TransportActor *> ActorHashmap;
  typedef __gnu_cxx::hash_map<core::uint64, core::uint64> TransportHashmap;

 public:
  bool Add(TransportActor *actor);
  TransportActor *Remove(core::uint64 id);
  TransportActor *Get(core::uint64 id);

  bool AddTransport(core::uint64 actor, core::uint64 transport_npc);
  core::uint64 RemoveTransport(core::uint64 actor);
  core::uint64 GetTransport(core::uint64 actor);

 private:
  TransportActorManager() {}
  ~TransportActorManager() {}

  ActorHashmap actors_;
  TransportHashmap transports_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__ACTOR__MANAGER__H

