//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 21:25:53.
// File name: transport_npc_manager.cc
//
// Description:
// Define class TransportNpcManager.
//

#include "game_server/server/extension/transport/transport_npc_manager.h"

#include "game_server/server/extension/transport/transport_npc.h"

namespace game {

namespace server {

namespace transport {

bool TransportNpcManager::Add(TransportNpc *npc) {
  if (npc == NULL) {
    return false;
  }

  NpcHashmap::const_iterator iterator = this->npcs_.find(npc->GetID());
  if (iterator != this->npcs_.end()) {
    return false;
  }

  this->npcs_.insert(std::make_pair(npc->GetID(), npc));

  return true;
}

TransportNpc *TransportNpcManager::Remove(core::uint64 id) {
  TransportNpc *npc = NULL;

  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if (iterator != this->npcs_.end()) {
    npc = iterator->second;
    this->npcs_.erase(iterator);
  }

  return npc;
}

TransportNpc *TransportNpcManager::Get(core::uint64 id) {
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if (iterator != this->npcs_.end()) {
    return iterator->second;
  }

  return NULL;
}

}  // namespace transport

}  // namespace server

}  // namespace game

