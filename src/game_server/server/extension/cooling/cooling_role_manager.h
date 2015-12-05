//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-26 18:54:29.
// File name: cooling_role_manager.h
//
// Description:
// Define class CoolingRoleManager.
//

#ifndef __GAME__SERVER__COOLING__ROLE__MANAGER__H
#define __GAME__SERVER__COOLING__ROLE__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace cooling {

class CoolingRole;

class CoolingRoleManager : public global::SingletonFactory<CoolingRoleManager> {
  friend class global::SingletonFactory<CoolingRoleManager>;

  typedef __gnu_cxx::hash_map<core::uint64, CoolingRole *> RoleHashMap;

 public:
  bool Initialize();
  void Finalize();

  // 玩家管理器相关操作
  CoolingRole *GetActor(core::uint64 id);
  bool AddActor(CoolingRole *actor);
  CoolingRole *RemoveActor(core::uint64 id);

  // NPC管理器相关操作
  CoolingRole *GetNpc(core::uint64 id);
  bool AddNpc(CoolingRole *npc);
  CoolingRole *RemoveNpc(core::uint64 id);

 private:
  CoolingRoleManager() {}
  ~CoolingRoleManager() {}

  // 把所有对象分开管理
  RoleHashMap actors_;
  RoleHashMap npcs_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__COOLING__ROLE__MANAGER__H

