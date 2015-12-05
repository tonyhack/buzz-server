//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:19:45.
// File name: effect_role_manager.h
//
// Description:
// Define class EffectRoleManager.
//

#ifndef __GAME__SERVER__EFFECT__EFFECT__ROLE__MANAGER__H
#define __GAME__SERVER__EFFECT__EFFECT__ROLE__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace effect {

class EffectRole;

class EffectRoleManager : public global::SingletonFactory<EffectRoleManager> {
  friend class global::SingletonFactory<EffectRoleManager>;

  typedef __gnu_cxx::hash_map<core::uint64, EffectRole *> RoleHashMap;

 public:
  bool Initialize();
  void Finalize();

  // 玩家管理器相关操作
  EffectRole *GetActor(core::uint64 id);
  bool AddActor(EffectRole *actor);
  EffectRole *RemoveActor(core::uint64 id);

  // NPC管理器相关操作
  EffectRole *GetNpc(core::uint64 id);
  bool AddNpc(EffectRole *npc);
  EffectRole *RemoveNpc(core::uint64 id);

 private:
  EffectRoleManager() {}
  ~EffectRoleManager() {}

  // 把所有对象分开管理
  RoleHashMap actors_;
  RoleHashMap npcs_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__EFFECT__EFFECT__ROLE__MANAGER__H

