//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-24 11:10:33.
// File name: scene_npc_pool.h
//
// Description:
// Define class SceneNpcPool.
//

#ifndef __GAME__SERVER__SCENE__SCENE__NPC__POOL__H
#define __GAME__SERVER__SCENE__SCENE__NPC__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneNpc;

class SceneNpcPool : public global::SingletonFactory<SceneNpcPool> {
  friend class global::SingletonFactory<SceneNpcPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SceneNpc *Allocate();
  void Deallocate(SceneNpc *npc);

 private:
  SceneNpcPool() {}
  ~SceneNpcPool() {}

  global::PoolTemplate<SceneNpc> npcs_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__NPC__POOL__H

