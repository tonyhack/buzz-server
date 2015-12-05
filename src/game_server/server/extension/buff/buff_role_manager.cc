//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:23:13.
// File name: buff_role_manager.cc
//
// Description:
// Define class BuffRoleManager.
//

#include "game_server/server/extension/buff/buff_role_manager.h"

#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_pool.h"

namespace game {

namespace server {

namespace buff {

bool BuffRoleManager::Initialize() {
  return true;
}

void BuffRoleManager::Finalize() {
  RoleHashMap::iterator iterator = this->actors_.begin();
  for(; iterator != this->actors_.begin(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      BuffRolePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->actors_.clear();

  iterator = this->npcs_.begin();
  for(; iterator != this->npcs_.begin(); ++iterator) {
    if(iterator->second) {
      iterator->second->Finalize();
      BuffRolePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->npcs_.clear();
}

BuffRole *BuffRoleManager::GetRole(entity::EntityType::type type, core::uint64 id) {
  if(type == entity::EntityType::TYPE_ACTOR) {
    return this->GetActor(id);
  } else if(type == entity::EntityType::TYPE_NPC) {
    return this->GetNpc(id);
  }

  return NULL;
}

BuffRole *BuffRoleManager::GetActor(core::uint64 id) {
  RoleHashMap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool BuffRoleManager::AddActor(BuffRole *actor) {
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

BuffRole *BuffRoleManager::RemoveActor(core::uint64 id) {
  BuffRole *role = NULL;
  RoleHashMap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    role = iterator->second;
    this->actors_.erase(iterator);
  }
  return role;
}

BuffRole *BuffRoleManager::GetNpc(core::uint64 id) {
  RoleHashMap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool BuffRoleManager::AddNpc(BuffRole *npc) {
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

BuffRole *BuffRoleManager::RemoveNpc(core::uint64 id) {
  BuffRole *role = NULL;
  RoleHashMap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    role = iterator->second;
    this->npcs_.erase(iterator);
  }
  return role;
}

}  // namespace buff

}  // namespace server

}  // namespace game

