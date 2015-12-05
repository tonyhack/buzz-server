//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-06 12:20:37.
// File name: scene_pool.h
//
// Description:
// Define class ScenePool.
//

#ifndef __SESSION__SERVER__SCENE__SCENE__POOL__H
#define __SESSION__SERVER__SCENE__SCENE__POOL__H

#include "core/base/noncopyable.h"
#include "global/pool_template.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace scene {

class Scene;

class ScenePool : public global::SingletonFactory<ScenePool> {
  friend class global::SingletonFactory<ScenePool>;

 public:
  bool Initialize(size_t initial_number, size_t extend_number);
  void Finalize();

  Scene *Allocate();
  void Deallocate(Scene *scene);

 private:
  ScenePool() {}
  ~ScenePool() {}

  global::PoolTemplate<Scene> scenes_;
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__SCENE__POOL__H

