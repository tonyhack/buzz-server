//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-03 10:06:51.
// File name: python_npc_manager.cc
//
// Description:
// Define class PythonNpcManager.
//

#include "game_server/server/extension/python/python_npc_manager.h"

#include "game_server/server/extension/python/python_npc.h"

namespace game {

namespace server {

namespace python {

PythonNpcPtr PythonNpcManager::Get(core::uint64 id) {
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }
  return PythonNpcPtr();
}

bool PythonNpcManager::Add(PythonNpcPtr &npc) {
  if(npc.get() == NULL) {
    return false;
  }
  NpcHashmap::iterator iterator = this->npcs_.find(npc->GetID());
  if(iterator != this->npcs_.end()) {
    return false;
  }
  this->npcs_.insert(std::make_pair(npc->GetID(), npc));
  return true;
}

PythonNpcPtr PythonNpcManager::Remove(core::uint64 id) {
  PythonNpcPtr npc;
  NpcHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    npc = iterator->second;
    this->npcs_.erase(iterator);
  }
  return npc;
}

}  // namespace python

}  // namespace server

}  // namespace game

