//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:39:10.
// File name: scene_builder.h
//
// Description:
// Define class SceneBuilder.
//

#ifndef __GAME__SERVER__SCENE__SCENE__BUILDER__H
#define __GAME__SERVER__SCENE__SCENE__BUILDER__H

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class Scene;

class SceneBuilder : public global::SingletonFactory<SceneBuilder> {
  friend class global::SingletonFactory<SceneBuilder>;

 public:
  Scene *CreateStatic(core::uint32 map, core::uint16 branch_id);
  Scene *CreateDynamic(core::uint32 map);

  void Destory(Scene *scene);

 private:
  SceneBuilder() {}
  ~SceneBuilder() {}
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__BUILDER__H

