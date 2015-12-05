//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-26 16:57:38.
// File name: scene_manager.h
//
// Description:
// Define class SceneManager.
//

#ifndef __SESSION__SERVER__SCENE__SCENE__MANAGER__H
#define __SESSION__SERVER__SCENE__SCENE__MANAGER__H

#include <map>
#include <set>

#include <ext/hash_map>

#include "core/base/types.h"
#include "core/timer_handler.h" 
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace scene {

class Scene;

class SceneManager : public global::SingletonFactory<SceneManager>, public core::TimerHandler {
  enum {
    SUMMON_BOSS,  // 召唤世界BOSS
  };

  friend class global::SingletonFactory<SceneManager>;

 public:
  typedef __gnu_cxx::hash_map<core::uint64, Scene *> SceneHashmap;
  typedef std::set<core::uint64> BranchSceneSet;
  typedef std::map<core::uint16, BranchSceneSet> BranchSceneMap;

  bool Initialize();
  void Finalize();

  bool AddScene(Scene *scene);
  Scene *RemoveScene(core::uint64 id);
  Scene *GetScene(core::uint64 id);

  Scene *GetBranchScene(core::uint16 map, core::uint16 branch);

  void TryBranchScene(core::uint16 map);

  bool AddBranchScene(core::uint16 map, core::uint64 scene);
  void RemoveBranchScene(core::uint16 map, core::uint64 scene);
  Scene *AllocateBranchScene(core::uint16 map);

  const BranchSceneSet *GetBranches(core::uint16 map);

  // 删除某个场景服务器下的所有场景
  void RemoveScenes(core::uint32 game_server);

  // 获取场景列表
  const SceneHashmap &GetScenes() const {
    return this->scenes_;
  }
  
  void StartExpireTimer(core::uint32 type);

  virtual void OnTimer(core::uint32 id);

 private:
  SceneManager() {}
  ~SceneManager() {}

  // 场景列表
  SceneHashmap scenes_;

  BranchSceneMap branches_;

  const static size_t MAX_BRANCH_MAP_NUMBER_ = 4;
  const static size_t MAX_MAP_ACTOR_NUMBER_ = 2;
};

}  // namespace scene

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SCENE__SCENE__MANAGER__H

