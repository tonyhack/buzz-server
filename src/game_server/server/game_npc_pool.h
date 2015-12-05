//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-19 16:50:58.
// File name: game_npc_pool.h
//
// Description:
// Define class GameNpcPool.
//

#ifndef __GAME__SERVER__GAME__NPC__POOL__H
#define __GAME__SERVER__GAME__NPC__POOL__H

#include "core/base/types.h"
#include "game_server/server/game_npc.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

class GameNpcPool : public global::SingletonFactory<GameNpcPool> {
  friend class global::SingletonFactory<GameNpcPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  GameNpc *Allocate();
  void Deallocate(GameNpc *npc);

 private:
  GameNpcPool() : sequence_(1) {}
  ~GameNpcPool() {}

  global::PoolTemplate<GameNpc> npcs_;

  core::uint64 sequence_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__NPC__POOL__H

