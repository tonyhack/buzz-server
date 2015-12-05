//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 10:04:02.
// File name: request_scene_get_game_server.cc
//
// Description:
// Define class RequestSceneGetGameServer.
//

#include "session_server/server/extension/scene/request_scene_get_game_server.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_scene_request_types.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_manager.h"

namespace session {

namespace server {

namespace scene {

RequestSceneGetGameServer::RequestSceneGetGameServer() {}
RequestSceneGetGameServer::~RequestSceneGetGameServer() {}

bool RequestSceneGetGameServer::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_GAME_SERVER,
      boost::bind(&RequestSceneGetGameServer::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneGetGameServer::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_GAME_SERVER);
}

int RequestSceneGetGameServer::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneGetGameServer) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneGetGameServer *request =
    (request::RequestSceneGetGameServer *)message;

  Scene *scene = NULL;

  if(request->__isset.map_ == true) {
    scene = SceneManager::GetInstance()->AllocateBranchScene(request->map_);
  } else {
    scene = SceneManager::GetInstance()->GetScene(request->scene_);
  }

  if(scene == NULL) {
    LOG_ERROR("获取 Scene 失败, request->map_=%d, request->scene_=%lu",
        request->map_, request->scene_);
    return -1;
  }

  request->__set_scene_(scene->GetID());
  request->__set_game_server_(scene->GetGameServer());

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace session

