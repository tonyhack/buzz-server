//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-08 12:08:31.
// File name: request_scene_relocation.cc
//
// Description:
// Define class RequestSceneRelocation.
//

#include "game_server/server/extension/scene/request_scene_relocation.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/extension/scene/facade_request.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneRelocation::RequestSceneRelocation() {}
RequestSceneRelocation::~RequestSceneRelocation() {}

bool RequestSceneRelocation::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_RELOCATION,
      boost::bind(&RequestSceneRelocation::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneRelocation::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_RELOCATION);
}

int RequestSceneRelocation::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneRelocation) != size) {
    global::LogError("%s:%d (%s) 请求重置场景位置时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneRelocation *request = (request::RequestSceneRelocation *)message;

  GameRole *role = FacadeRequest::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求场景跳转时，找不到角色",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(role->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求角色跳转时，所在场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, role->GetScene());
    return -1;
  }

  if(scene->Jump(role, request->x_, request->y_)) {
    request->__set_result_(true);
  } else {
    request->__set_result_(false);
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

