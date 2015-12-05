//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-17 11:51:26.
// File name: request_scene_daynightinfo.cc
//
// Description:
// Define class RequestSceneDayNightInfo.
//

#include "game_server/server/extension/scene/request_scene_daynightinfo.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_daynight.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneDayNightInfo::RequestSceneDayNightInfo() {}
RequestSceneDayNightInfo::~RequestSceneDayNightInfo() {}

bool RequestSceneDayNightInfo::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_DAYNIGHT_INFO,
      boost::bind(&RequestSceneDayNightInfo::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneDayNightInfo::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_DAYNIGHT_INFO);
}

int RequestSceneDayNightInfo::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneDayNightInfo) != size) {
    global::LogError("%s:%d (%s) 请求场景昼夜信息时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneDayNightInfo *request = (request::RequestSceneDayNightInfo *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求场景昼夜信息时，找不到场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, request->scene_);
    return -1;
  }
  
  SceneDayNight &daynight = scene->GetSceneDayNight();
  request->__set_status_(static_cast<request::DayNightStatusType::type>(daynight.GetCurStatus()));
  request->__set_aoi_coe_(daynight.GetAoiCoe());

  return 0;
}


}  // namespace scene

}  // namespace server

}  // namespace game

