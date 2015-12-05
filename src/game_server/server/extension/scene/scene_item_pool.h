//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-12 18:16:24.
// File name: scene_item_pool.h
//
// Description:
// Define class SceneItemPool.
//

#ifndef __GAME__SERVER__SCENE__SCENE__ITEM__POOL__H
#define __GAME__SERVER__SCENE__SCENE__ITEM__POOL__H

#include "core/base/types.h"
#include "game_server/server/game_npc.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneItem;

class SceneItemPool : public global::SingletonFactory<SceneItemPool> {
  friend class global::SingletonFactory<SceneItemPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SceneItem *Allocate();
  void Deallocate(SceneItem *item);

 private:
  SceneItemPool() : sequence_(1) {}
  ~SceneItemPool() {}

  global::PoolTemplate<SceneItem> items_;

  core::uint64 sequence_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ITEM__POOL__H

