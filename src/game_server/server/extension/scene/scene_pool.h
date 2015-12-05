//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-14 17:28:00.
// File name: scene_pool.h
//
// Description:
// Define class ScenePool.
//

#ifndef __GAME__SERVER__SCENE__SCENE__POOL__H
#define __GAME__SERVER__SCENE__SCENE__POOL__H

#include <time.h>

#include <list>
#include <map>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace scene {

class Scene;

// 场景信息
class SceneInfo {
 public:
  SceneInfo() : scene_(NULL), time_(0) {}
  ~SceneInfo() {}

  Scene *scene_;
  time_t time_;
};

// 场景对象池
class ScenePool : public global::SingletonFactory<ScenePool>,
  public core::TimerHandler {
  friend class global::SingletonFactory<ScenePool>;

  typedef std::list<SceneInfo> SceneList;
  typedef std::map<core::uint16, SceneList> SceneListMap;

 public:
  bool Initialize();
  void Finalize();

  Scene *Allocate(core::uint16 map);
  void Deallocate(Scene *scene);

  virtual void OnTimer(core::uint32 id);

 private:
  ScenePool();
  virtual ~ScenePool();

  void Reduce();

  // 场景列表
  SceneListMap scene_lists_;

  // 在对象池中等待销毁的时间(单位为秒)
  const static time_t deallocate_time_ = 60 * 60;
  const static core::uint32 check_deallocate_time_ = 10 * 60;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__POOL__H

