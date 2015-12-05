//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 11:25:42.
// File name: scene_npc_manager.cc
//
// Description:
// Define class SceneNpcManager.
//

#include "game_server/server/extension/scene/scene_npc_manager.h"

#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_pool.h"

namespace game {

namespace server {

namespace scene {

bool SceneNpcManager::Initialize() {
  return true;
}

void SceneNpcManager::Finalize() {
  NpcHashmap::iterator iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      SceneNpcPool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->npcs_.clear();
}

SceneNpc *SceneNpcManager::Get(core::uint64 id) {
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool SceneNpcManager::Add(SceneNpc *npc) {
  if(npc == NULL) {
    return false;
  }
  NpcHashmap::const_iterator iterator = this->npcs_.find(npc->GetID());
  if(iterator != this->npcs_.end()) {
    return false;
  }
  this->npcs_.insert(std::make_pair(npc->GetID(), npc));

  return true;
}

SceneNpc *SceneNpcManager::Remove(core::uint64 id) {
  SceneNpc *npc = NULL;
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    npc = iterator->second;
    this->npcs_.erase(iterator);
  }
  return npc;
}

}  // namespace scene

}  // namespace server

}  // namespace game

