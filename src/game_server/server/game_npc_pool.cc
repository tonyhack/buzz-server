//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 16:54:08.
// File name: game_npc_pool.cc
//
// Description:
// Define class GameNpcPool.
//

#include "game_server/server/game_npc_pool.h"

namespace game {

namespace server {

bool GameNpcPool::Initialize(size_t initial_number, size_t extend_number) {
  return this->npcs_.Initialize(initial_number, extend_number);
}

void GameNpcPool::Finalize() {
  this->npcs_.Finalize();
}

GameNpc *GameNpcPool::Allocate() {
  GameNpc *npc = this->npcs_.Allocate();
  if(npc) {
    npc->Allocate(this->sequence_++);
  }
  return npc;
}

void GameNpcPool::Deallocate(GameNpc *npc) {
  if(npc) {
    npc->Deallocate();
    this->npcs_.Deallocate(npc);
  }
}

}  // namespace server

}  // namespace game

