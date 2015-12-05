//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-01-25 02:01:24.
// File name: summon_npc_manager.cc
//
// Description:
//
#include "game_server/server/extension/task/summon_npc_manager.h"

namespace game {

namespace server {

namespace task {

SummonNpcManager::SummonNpcManager() {}
SummonNpcManager::~SummonNpcManager() {}

bool SummonNpcManager::Initialize() {
  this->npcs_.clear();
  return true;
}

void SummonNpcManager::Finalize() {
  this->npcs_.clear();
}

bool SummonNpcManager::RemoveNpc(core::int32 npc_template, 
    core::uint64 scene_id) {
  Key key(scene_id, npc_template);
  SceneNpcMap::iterator itr = this->npcs_.find(key);
  if(itr == this->npcs_.end()) {
    return false;
  }
  this->npcs_.erase(itr);
  return true;
}

bool SummonNpcManager::CheckNpc(core::int32 npc_template, 
    core::uint64 scene_id) {
  Key key(scene_id, npc_template);
  SceneNpcMap::iterator itr = this->npcs_.find(key);
  if(itr == this->npcs_.end()) {
    return false;
  }
  return true;
}

bool SummonNpcManager::AddNpc(core::int32 npc_template, core::uint64 scene_id,
    core::uint64 npc_id) {
  Key key(scene_id, npc_template);
  SceneNpcMap::iterator itr = this->npcs_.find(key);
  if(itr != this->npcs_.end()) {
    return false;
  }
  this->npcs_.insert(std::make_pair(key, npc_id));

  return true;
}

} // namepsace task

} // namespace server

} // namespace game
