//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-12 11:12:22.
// File name: scene_actor_pool.h
//
// Description:
// Define class SceneActorPool.
//

#ifndef __GAME__SERVER__SCENE__SCENE__ACTOR__POOL__H
#define __GAME__SERVER__SCENE__SCENE__ACTOR__POOL__H

#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneActor;

class SceneActorPool : public global::SingletonFactory<SceneActorPool> {
  friend class global::SingletonFactory<SceneActorPool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  SceneActor *Allocate();
  void Deallocate(SceneActor *actor);

 private:
  SceneActorPool() {}
  ~SceneActorPool() {}

  global::PoolTemplate<SceneActor> actors_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ACTOR__POOL__H

