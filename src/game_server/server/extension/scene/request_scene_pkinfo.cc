//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-18 13:38:26.
// File name: request_scene_pkinfo.cc
//
// Description:
// Define class RequestScenePKInfo.
//

#include "game_server/server/extension/scene/request_scene_pkinfo.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestScenePKInfo::RequestScenePKInfo() {}
RequestScenePKInfo::~RequestScenePKInfo() {}

bool RequestScenePKInfo::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_PK_INFO,
      boost::bind(&RequestScenePKInfo::OnRequest, this, _1, _2));
  return true;
}

void RequestScenePKInfo::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_PK_INFO);
}

int RequestScenePKInfo::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestScenePKInfo) != size) {
    global::LogError("%s:%d (%s) 请求角色移动时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestScenePKInfo *request = (request::RequestScenePKInfo *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求角色PK时，找不到场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, request->scene_);
    return -1;
  }

  request->__set_rookie_protect_(scene->IsRookieProtect());
  request->__set_ban_change_pk_mode_(scene->IsBanChangePKMode());
  request->__set_die_protect_(scene->IsDieProtect());
  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

