//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-13 11:51:26.
// File name: request_scene_jump.cc
//
// Description:
// Define class RequestSceneJump.
//

#include "game_server/server/extension/scene/request_scene_jump.h"

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

RequestSceneJump::RequestSceneJump() {}
RequestSceneJump::~RequestSceneJump() {}

bool RequestSceneJump::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_JUMP,
      boost::bind(&RequestSceneJump::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneJump::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_JUMP);
}

int RequestSceneJump::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneJump) != size) {
    global::LogError("%s:%d (%s) 请求场景跳转时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneJump *request = (request::RequestSceneJump *)message;

  GameRole *role = FacadeRequest::GetInstance()->GetRole(request->type_, request->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 请求场景跳转时，找不到角色",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(role->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请场景跳转时，所在场景(%lu)",
        __FILE__, __LINE__, __FUNCTION__, role->GetScene());
    return -1;
  }

  bool result_ = false;

  if (request->__isset.map_ == true &&
      scene->JumpMap(role, request->map_, request->x_, request->y_)) {
    request->__set_result_(true);
  } else if (request->__isset.scene_ == true &&
      scene->JumpScene(role, request->scene_, request->x_, request->y_)) {
    request->__set_result_(true);
  } else {
    request->__set_result_(false);
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

