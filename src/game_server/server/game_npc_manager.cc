//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 17:10:22.
// File name: game_npc_manager.cc
//
// Description:
// Define class GameNpcManager.
//

#include "game_server/server/game_npc_manager.h"

#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_pool.h"
#include "global/logging.h"

namespace game {

namespace server {

bool GameNpcManager::Initialize() {
  return true;
}

void GameNpcManager::Finalize() {
  NpcHashmap::iterator iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      GameNpcPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->npcs_.clear();
}

GameNpc *GameNpcManager::Get(core::uint64 id) {
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool GameNpcManager::Add(GameNpc *npc) {
  if(npc == NULL) {
    global::LogError("%s:%d (%s) npc is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  NpcHashmap::iterator iterator1 = this->npcs_.find(npc->GetID());
  if(iterator1 != this->npcs_.end()) {
    return false;
  }
  this->npcs_.insert(std::make_pair(npc->GetID(), npc));
  return true;
}

GameNpc *GameNpcManager::Remove(core::uint64 id) {
  NpcHashmap::iterator iterator1 = this->npcs_.find(id);
  if(iterator1 == this->npcs_.end()) {
    return NULL;
  }

  GameNpc *npc = iterator1->second;
  if(npc == NULL) {
    global::LogError("%s:%d (%s) Npc(%lld) is null",
        __FILE__, __LINE__, __FUNCTION__, id);
    return NULL;
  }

  this->npcs_.erase(iterator1);

  return npc;
}

}  // namespace server

}  // namespace game

