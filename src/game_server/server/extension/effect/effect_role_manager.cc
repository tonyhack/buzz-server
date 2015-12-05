//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:23:13.
// File name: effect_role_manager.cc
//
// Description:
// Define class EffectRoleManager.
//

#include "game_server/server/extension/effect/effect_role_manager.h"

#include "game_server/server/extension/effect/effect_role.h"
#include "game_server/server/extension/effect/effect_role_pool.h"

namespace game {

namespace server {

namespace effect {

bool EffectRoleManager::Initialize() {
  return true;
}

void EffectRoleManager::Finalize() {
  RoleHashMap::iterator iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      EffectRolePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->npcs_.clear();

  iterator = this->actors_.begin();
  for(; iterator != this->actors_.end(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      EffectRolePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->actors_.clear();
}

EffectRole *EffectRoleManager::GetActor(core::uint64 id) {
  RoleHashMap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool EffectRoleManager::AddActor(EffectRole *actor) {
  // 检测指针是否合法
  if(actor == NULL) {
    return false;
  }
  // 检测类型是否正确
  if(actor->GetType() != entity::EntityType::TYPE_ACTOR) {
    return false;
  }
  // 检测是否存在
  RoleHashMap::const_iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }
  // 加入管理器中
  this->actors_.insert(std::make_pair(actor->GetID(), actor));
  return true;
}

EffectRole *EffectRoleManager::RemoveActor(core::uint64 id) {
  EffectRole *role = NULL;
  RoleHashMap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    role = iterator->second;
    this->actors_.erase(iterator);
  }
  return role;
}

EffectRole *EffectRoleManager::GetNpc(core::uint64 id) {
  RoleHashMap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool EffectRoleManager::AddNpc(EffectRole *npc) {
  // 检测指针是否合法
  if(npc == NULL) {
    return false;
  }
  // 检测类型是否正确
  if(npc->GetType() != entity::EntityType::TYPE_NPC) {
    return false;
  }
  // 检测是否存在
  RoleHashMap::const_iterator iterator = this->npcs_.find(npc->GetID());
  if(iterator != this->npcs_.end()) {
    return false;
  }
  // 加入管理器中
  this->npcs_.insert(std::make_pair(npc->GetID(), npc));
  return true;
}

EffectRole *EffectRoleManager::RemoveNpc(core::uint64 id) {
  EffectRole *role = NULL;
  RoleHashMap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    role = iterator->second;
    this->npcs_.erase(iterator);
  }
  return role;
}

}  // namespace effect

}  // namespace server

}  // namespace game

