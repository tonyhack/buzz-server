//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 21:15:38.
// File name: transport_npc_pool.cc
//
// Description:
// Define class TransportNpcPool.
//

#include "game_server/server/extension/transport/transport_npc_pool.h"

#include "game_server/server/extension/transport/transport_npc.h"

namespace game {

namespace server {

namespace transport {

bool TransportNpcPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->npcs_.Initialize(initial_number, extend_number);
}

void TransportNpcPool::Finalize() {
  this->npcs_.Finalize();
}

TransportNpc *TransportNpcPool::Allocate() {
  return this->npcs_.Allocate();
}

void TransportNpcPool::Deallocate(TransportNpc *npc) {
  this->npcs_.Deallocate(npc);
}

}  // namespace transport

}  // namespace server

}  // namespace game

