//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-22 18:26:03.
// File name: request_scene_get_npcs.cc
//
// Description:
// Define class RequestSceneGetNpcs.
//

#include "game_server/server/extension/scene/request_scene_get_npcs.h"

#include <boost/bind.hpp>

#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneGetNpcs::RequestSceneGetNpcs() {}
RequestSceneGetNpcs::~RequestSceneGetNpcs() {}

bool RequestSceneGetNpcs::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_NPCS,
      boost::bind(&RequestSceneGetNpcs::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneGetNpcs::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
    request::RequestType::REQUEST_SCENE_GET_NPCS);
}

int RequestSceneGetNpcs::OnRequest(void *data, size_t size) {
  if (data == NULL || sizeof(request::RequestSceneGetNpcs) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneGetNpcs *request = (request::RequestSceneGetNpcs *)data;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if (scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", request->scene_);
    return -1;
  }

  scene->GetNpcs(request->npcs_);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

