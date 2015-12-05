//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-04 10:31:07.
// File name: skill_role_manager.h
//
// Description:
// Define class SkillRoleManager.
//

#ifndef __GAME__SERVER__SKILL__SKILL__ROLE__MANAGER__H
#define __GAME__SERVER__SKILL__SKILL__ROLE__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace skill {

class SkillRole;

class SkillRoleManager : public global::SingletonFactory<SkillRoleManager> {
  friend class global::SingletonFactory<SkillRoleManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SkillRole *> RoleHashmap;

 public:
  // 加入管理器
  bool Add(SkillRole *role);
  SkillRole *Get(entity::EntityType::type type, core::uint64 id);

  // 玩家对象的 增加/获取/移除
  bool AddActor(SkillRole *actor);
  SkillRole *GetActor(core::uint64 id);
  SkillRole *RemoveActor(core::uint64 id);

  // npc对象的 增加/获取/移除
  bool AddNpc(SkillRole *npc);
  SkillRole *GetNpc(core::uint64 id);
  SkillRole *RemoveNpc(core::uint64 id);

 private:
  SkillRoleManager() {}
  ~SkillRoleManager() {}

  // 玩家表
  RoleHashmap actors_;
  // npc表
  RoleHashmap npcs_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__SKILL__ROLE__MANAGER__H

