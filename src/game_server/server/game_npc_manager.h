//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 17:04:15.
// File name: game_npc_manager.h
//
// Description:
// Define class GameNpcManager.
//

#ifndef __GAME__SERVER__GAME__NPC__MANAGER__H
#define __GAME__SERVER__GAME__NPC__MANAGER__H

#include <map>

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/hash_string.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameNpc;

class GameNpcManager : public global::SingletonFactory<GameNpcManager> {
  friend class global::SingletonFactory<GameNpcManager>;

  typedef __gnu_cxx::hash_map<core::uint64, GameNpc *> NpcHashmap;

 public:
  bool Initialize();
  void Finalize();

  GameNpc *Get(core::uint64 id);

  bool Add(GameNpc *npc);
  GameNpc *Remove(core::uint64 id);

 private:
  GameNpcManager() {}
  ~GameNpcManager() {}

  NpcHashmap npcs_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__NPC__MANAGER__H

