//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-19 10:38:26.
// File name: request_scene_issafe.cc
//
// Description:
// Define class RequestSceneIsSafe.
//

#include "game_server/server/extension/scene/request_scene_issafe.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneIsSafe::RequestSceneIsSafe() {}
RequestSceneIsSafe::~RequestSceneIsSafe() {}

bool RequestSceneIsSafe::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_IS_SAFE,
      boost::bind(&RequestSceneIsSafe::OnRequest, this, _1, _2));
  return true;
}

void RequestSceneIsSafe::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_IS_SAFE);
}

int RequestSceneIsSafe::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneIsSafe) != size) {
    global::LogError("%s:%d (%s) 请求角色移动时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneIsSafe *request = (request::RequestSceneIsSafe *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求角色PK时，找不到场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, request->scene_);
    return -1;
  }

  request->__set_is_safe_(scene->CheckSafeArea(request->x_, request->y_));
  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

