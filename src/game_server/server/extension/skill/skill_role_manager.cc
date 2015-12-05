//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 10:46:07.
// File name: skill_role_manager.cc
//
// Description:
// Define class SkillRoleManager.
//

#include "game_server/server/extension/skill/skill_role_manager.h"

#include "game_server/server/extension/skill/skill_role.h"

namespace game {

namespace server {

namespace skill {

bool SkillRoleManager::Add(SkillRole *role) {
  if(role == NULL) {
    return false;
  }

  switch(role->GetType()) {
    case entity::EntityType::TYPE_ACTOR:
      return this->AddActor(role);
    case entity::EntityType::TYPE_NPC:
      return this->AddNpc(role);
    default:
      break;
  }

  return false;
}

SkillRole *SkillRoleManager::Get(entity::EntityType::type type, core::uint64 id) {
  if(type == entity::EntityType::TYPE_ACTOR) {
    return this->GetActor(id);
  } else if(type == entity::EntityType::TYPE_NPC) {
    return this->GetNpc(id);
  } else {
    return NULL;
  }
}

bool SkillRoleManager::AddActor(SkillRole *actor) {
  if(actor == NULL) {
    return false;
  }

  RoleHashmap::iterator iterator = this->actors_.find(actor->GetID());
  if(iterator != this->actors_.end()) {
    return false;
  }

  this->actors_.insert(std::make_pair(actor->GetID(), actor));

  return true;
}

SkillRole *SkillRoleManager::GetActor(core::uint64 id) {
  RoleHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    return iterator->second;
  }

  return NULL;
}

SkillRole *SkillRoleManager::RemoveActor(core::uint64 id) {
  SkillRole *actor = NULL;

  RoleHashmap::iterator iterator = this->actors_.find(id);
  if(iterator != this->actors_.end()) {
    actor = iterator->second;
    this->actors_.erase(iterator);
  }

  return actor;
}

bool SkillRoleManager::AddNpc(SkillRole *npc) {
  if(npc == NULL) {
    return false;
  }

  RoleHashmap::iterator iterator = this->npcs_.find(npc->GetID());
  if(iterator != this->npcs_.end()) {
    return false;
  }

  this->npcs_.insert(std::make_pair(npc->GetID(), npc));

  return true;
}

SkillRole *SkillRoleManager::GetNpc(core::uint64 id) {
  RoleHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    return iterator->second;
  }

  return NULL;
}

SkillRole *SkillRoleManager::RemoveNpc(core::uint64 id) {
  SkillRole *npc = NULL;

  RoleHashmap::iterator iterator = this->npcs_.find(id);
  if(iterator != this->npcs_.end()) {
    npc = iterator->second;
    this->npcs_.erase(iterator);
  }

  return npc;
}

}  // namespace skill

}  // namespace server

}  // namespace game

