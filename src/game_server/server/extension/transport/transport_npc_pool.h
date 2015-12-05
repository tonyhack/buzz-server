//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 20:58:11.
// File name: transport_npc_poo.h
//
// Description:
// Define class TransportNpcPool.
//

#ifndef __GAME__SERVER__TRANSPORT__NPC__POOL__H
#define __GAME__SERVER__TRANSPORT__NPC__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class TransportNpc;

class TransportNpcPool : public global::SingletonFactory<TransportNpcPool> {
  friend class global::SingletonFactory<TransportNpcPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  TransportNpc *Allocate();
  void Deallocate(TransportNpc *npc);

 private:
  TransportNpcPool() {}
  ~TransportNpcPool() {}

  global::PoolTemplate<TransportNpc> npcs_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__NPC__POOL__H

