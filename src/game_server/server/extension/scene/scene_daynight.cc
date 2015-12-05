//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-03-12 10:01:05.
// File name: scene_daynight.cc
//
// Description:
// Define class SceneActor.
//

#include "game_server/server/extension/scene/scene_daynight.h"

#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/scene.h"

namespace game {

namespace server {

namespace scene {

SceneDayNight::SceneDayNight() : scene_(NULL), map_info_(NULL),
  cur_status_(STATUS_MORNING), cur_aoi_coe_(0){}
SceneDayNight::~SceneDayNight() {}

bool SceneDayNight::Initialize(Scene *scene) {
  this->scene_ = scene;

  const ServerMapConfigure *server_map_configure =  
    Configure::GetInstance()->GetServerMapConfigure(); 
  if(server_map_configure == NULL) {   
    global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
        __FILE__, __LINE__, __FUNCTION__);
    return false; 
  }
  const MapInfo *map_info = server_map_configure->GetMapInfo(this->scene_->GetMapID());
  if(map_info == NULL) {   
    global::LogError("%s:%d (%s) 找不到地图[%d]信息配置",
        __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
    return false;
  }

  // 查看当前scene配置 
  core::int32 totaldayminute = map_info->GetDayNight().morning_interval_ + 
    map_info->GetDayNight().daytime_interval_ + map_info->GetDayNight().evening_interval_ +
    map_info->GetDayNight().night_interval_;
  core::int32 morning = map_info->GetDayNight().morning_interval_ * 60;
  core::int32 daytime = morning + map_info->GetDayNight().daytime_interval_ * 60;
  core::int32 evening = daytime + map_info->GetDayNight().evening_interval_ * 60;
  core::int32 night = evening + map_info->GetDayNight().night_interval_ * 60;

  if(totaldayminute <= 0) {
    global::LogError("%s:%d (%s) 地图[%d]昼夜信息配置错误",
        __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
    return false;
  }

  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  struct tm *ptr;
  time_t lt;
  lt =time(NULL);
  ptr=localtime(&lt);
  ptr->tm_hour = 0;
  ptr->tm_min = 0;
  ptr->tm_sec = 0;
  // 当前日期的零点时间
  time_t today_zero = mktime(ptr);
  core::int32 curinterval =  (now - today_zero)%(totaldayminute * 60);
  core::int32 add_time_interval = 0;
  if(curinterval >= 0 && curinterval < morning) {
    add_time_interval =morning - curinterval;
    this->cur_status_ = STATUS_MORNING;
    this->cur_aoi_coe_ = map_info->GetDayNight().morning_aoi_coe_;
  }
  else if(curinterval >= morning && curinterval < daytime) {
    add_time_interval = daytime - curinterval;
    this->cur_status_ = STATUS_DAYTIME;
    this->cur_aoi_coe_ = map_info->GetDayNight().daytime_aoi_coe_;
  }
  else if(curinterval >= daytime && curinterval < evening) {
    add_time_interval = evening - curinterval;
    this->cur_status_ = STATUS_EVENING;
    this->cur_aoi_coe_ = map_info->GetDayNight().evening_aoi_coe_;
  }
  else if(curinterval >= evening && curinterval < night) {
    add_time_interval = night - curinterval;
    this->cur_status_ = STATUS_NIGHT;
    this->cur_aoi_coe_ = map_info->GetDayNight().night_aoi_coe_;
  }
  else {
    global::LogError("%s:%d (%s) 地图[%d]昼夜信息配置错误",
        __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
    return false;
  }
  this->map_info_ = map_info;

  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DAYNIGHT, add_time_interval * 1000,
      -1, this, "SceneDayNight::Initialize");


  return true;
}

void SceneDayNight::Finalize() {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_DAYNIGHT, this);
}


void SceneDayNight::OnTimer(core::uint32 id) {
  if(TIMER_ID_DAYNIGHT == id)
    this->ChangeStatus();

}

void SceneDayNight::ChangeStatus() {
  if(this->map_info_ == NULL)
    return;

  // 删除原来的定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_DAYNIGHT, this);
  // 查看当前状态
  switch(this->cur_status_) {
    case STATUS_MORNING:
      this->cur_status_ = STATUS_DAYTIME;
      this->cur_aoi_coe_ = map_info_->GetDayNight().daytime_aoi_coe_;
      GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DAYNIGHT, map_info_->GetDayNight().daytime_interval_ * 60 * 1000,
          -1, this, "SceneDayNight::ChangeStatus");
      break;
    case STATUS_DAYTIME:
      this->cur_status_ = STATUS_EVENING;
      this->cur_aoi_coe_ = map_info_->GetDayNight().evening_aoi_coe_;
      GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DAYNIGHT, map_info_->GetDayNight().evening_interval_ * 60 * 1000,
          -1, this, "SceneDayNight::ChangeStatus");
      break;
    case STATUS_EVENING:
      this->cur_status_ = STATUS_NIGHT;
      this->cur_aoi_coe_ = map_info_->GetDayNight().night_aoi_coe_;
      GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DAYNIGHT, map_info_->GetDayNight().night_interval_ * 60 * 1000,
          -1, this, "SceneDayNight::ChangeStatus");
      break;
    case STATUS_NIGHT:
      this->cur_status_ = STATUS_MORNING;
      this->cur_aoi_coe_ = map_info_->GetDayNight().morning_aoi_coe_;
      GameServerSingleton::GetInstance().AddTimer(TIMER_ID_DAYNIGHT, map_info_->GetDayNight().morning_interval_ * 60 * 1000,
          -1, this, "SceneDayNight::ChangeStatus");
      break;
    default:
      return;
  }

  if(this->scene_) {
    this->scene_->BroadcastDayNightChange();
  }
}


}  // namespace scene

}  // namespace server

}  // namespace game

