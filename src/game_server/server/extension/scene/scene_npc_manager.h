//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 11:15:30.
// File name: scene_npc_manager.h
//
// Description:
// Define class SceneNpcManager.
//

#ifndef __GAME__SERVER__SCENE__SCENE__NPC__MANAGER__H
#define __GAME__SERVER__SCENE__SCENE__NPC__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneNpc;

class SceneNpcManager : public global::SingletonFactory<SceneNpcManager> {
  friend class global::SingletonFactory<SceneNpcManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SceneNpc *> NpcHashmap;

 public:
  bool Initialize();
  void Finalize();

  SceneNpc *Get(core::uint64 id);
  bool Add(SceneNpc *npc);
  SceneNpc *Remove(core::uint64 id);

 private:
  SceneNpcManager() {}
  ~SceneNpcManager() {}

  NpcHashmap npcs_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__NPC__MANAGER__H

