//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-13 11:32:58.
// File name: scene_item_manager.h
//
// Description:
// Define class SceneItemManager.
//

#ifndef __GAME__SERVER__SCENE__SCENE__ITEM__MANAGER__H
#define __GAME__SERVER__SCENE__SCENE__ITEM__MANAGER__H

#include <ext/hash_map>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class SceneItem;

class SceneItemManager : public global::SingletonFactory<SceneItemManager> {
  friend class global::SingletonFactory<SceneItemManager>;

  typedef __gnu_cxx::hash_map<core::uint64, SceneItem *> ItemHashmap;

 public:
  SceneItem *Get(core::uint64 id);
  bool Add(SceneItem *item);
  SceneItem *Remove(core::uint64 id);

 private:
  SceneItemManager() {}
  ~SceneItemManager() {}

  ItemHashmap items_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__ITEM__MANAGER__H

