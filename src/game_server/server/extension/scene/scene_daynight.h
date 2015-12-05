//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-14 17:14:00.
// File name: scene_daynight.h
//
// Description:
// Define class SceneActor.
//

#ifndef __GAME__SERVER__SCENE__SCENE__DAYNIGHT__H
#define __GAME__SERVER__SCENE__SCENE__DAYNIGHT__H

#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"
#include "core/base/types.h"
#include "core/timer_handler.h"

namespace game {

namespace server {

namespace scene {

class Scene;
class MapInfo;

enum Status{ STATUS_MORNING = 0, STATUS_DAYTIME, STATUS_EVENING, STATUS_NIGHT,};
 
class SceneDayNight : public core::Noncopyable,
 public core::TimerHandler {
    enum { TIMER_ID_DAYNIGHT = 0, };
 public:
    SceneDayNight();
    virtual ~SceneDayNight();
 
    bool Initialize(Scene *scene);
    void Finalize();

    inline Status GetCurStatus() const {
      return this->cur_status_;
    }

    inline core::int32 GetAoiCoe() const {
      return this->cur_aoi_coe_;
    }
    
    virtual void OnTimer(core::uint32 id);
   
    void ChangeStatus();
 private:
    // 所在场景
    Scene *scene_;
    // 当前地图配置信息
    const MapInfo* map_info_;
    // 当前昼夜状态
    Status cur_status_;
    // 当前视野系数
    core::int32 cur_aoi_coe_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__SCENE__DAYNIGHT__H

