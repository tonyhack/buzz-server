//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:19:45.
// File name: buff_role_manager.h
//
// Description:
// Define class BuffRoleManager.
//

#ifndef __GAME__SERVER__BUFF__BUFF__ROLE__MANAGER__H
#define __GAME__SERVER__BUFF__BUFF__ROLE__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "entity/entity_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace buff {

class BuffRole;

class BuffRoleManager : public global::SingletonFactory<BuffRoleManager> {
  friend class global::SingletonFactory<BuffRoleManager>;

  typedef __gnu_cxx::hash_map<core::uint64, BuffRole *> RoleHashMap;

 public:
  bool Initialize();
  void Finalize();

  BuffRole *GetRole(entity::EntityType::type type, core::uint64 id);

  // 玩家管理器相关操作
  BuffRole *GetActor(core::uint64 id);
  bool AddActor(BuffRole *actor);
  BuffRole *RemoveActor(core::uint64 id);

  // NPC管理器相关操作
  BuffRole *GetNpc(core::uint64 id);
  bool AddNpc(BuffRole *npc);
  BuffRole *RemoveNpc(core::uint64 id);

 private:
  BuffRoleManager() {}
  ~BuffRoleManager() {}

  // 把所有对象分开管理
  RoleHashMap actors_;
  RoleHashMap npcs_;
};

}  // namespace cooling

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__BUFF__ROLE__MANAGER__H

