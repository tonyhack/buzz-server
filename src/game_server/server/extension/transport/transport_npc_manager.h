//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-08 22:38:06.
// File name: transport_npc_manager.h
//
// Description:
// Define class TransportNpcManager.
//

#ifndef __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__MANAGER__H
#define __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__MANAGER__H

#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace transport {

class TransportNpc;

class TransportNpcManager : public global::SingletonFactory<TransportNpcManager> {
  friend class global::SingletonFactory<TransportNpcManager>;
  typedef __gnu_cxx::hash_map<core::uint64, TransportNpc *> NpcHashmap;

 public:
  bool Add(TransportNpc *npc);
  TransportNpc *Remove(core::uint64 id);
  TransportNpc *Get(core::uint64 id);

 private:
  TransportNpcManager() {}
  ~TransportNpcManager() {}

  NpcHashmap npcs_;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__TRANSPORT__NPC__MANAGER__H

