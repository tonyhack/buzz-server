//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-08 13:46:47.
// File name: scene_manager.h
//
// Description:
// Define class SceneManager.
//

#ifndef __GAME__SERVER__SCENE__SCENE__MANAGER__H
#define __GAME__SERVER__SCENE__SCENE__MANAGER__H

#include <map>
#include <set>

#include "core/base/types.h"
#include "global/sequence_allocator.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class Scene;

class SceneManager : public global::SingletonFactory<SceneManager> {
  friend class global::SingletonFactory<SceneManager>;

 public:
  typedef std::map<core::uint64, Scene *> SceneMap;

  bool Initialize();
  void Finalize();
 
  // 取得场景
  Scene *GetScene(core::uint64 id);

  // 场景对象加入管理器
  bool AddScene(Scene *scene);
  // 从管理器中删除场景对象
  Scene *RemoveScene(core::uint64 id);

  // 生成一个静态地图ID
  core::uint64 GenerateStaticSceneID(core::uint16 map_id, core::uint16 branch_id);

  // 分配一个动态地图ID
  core::uint64 AllocateSceneID(core::uint16 map_id);
  // 回收一个场景ID
  void DeallocateSceneID(core::uint64 scene_id);

  // 得到所有场景集合
  inline const SceneMap &GetScenes() const {
    return this->scenes_;
  }

 private:
  SceneManager() {}
  ~SceneManager() {}

  void GenerateSceneID(core::uint16 map_id, core::uint16 sequence,
      core::uint64 &scene_id);

  SceneMap scenes_;

  global::CommonSmallSequenceAllocator sequences_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__MANAGER__H

