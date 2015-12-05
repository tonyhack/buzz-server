//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-29 12:10:09.
// File name: request_get_map_scene_id.cc
//
// Description:
// Define RequestGetMapSceneID.
//

#include "session_server/server/extension/scene/request_get_map_scene_id.h"

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

RequestGetMapSceneID::RequestGetMapSceneID() {}
RequestGetMapSceneID::~RequestGetMapSceneID() {}

bool RequestGetMapSceneID::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GET_MAP_SCENE_ID,
      boost::bind(&RequestGetMapSceneID::OnRequest, this, _1, _2));

  return true;
}

void RequestGetMapSceneID::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GET_MAP_SCENE_ID);
}

int RequestGetMapSceneID::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestGetMapSceneID) != size) {
    LOG_ERROR("请求获取地图的场景ID时，参数错误");
    return -1;
  }

  request::RequestGetMapSceneID *request = (request::RequestGetMapSceneID *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->map_);
  if(scene == NULL) {
    LOG_ERROR("静态场景(%u) 不存在", request->map_);
    return -1;
  }

  request->__set_scene_(scene->GetID());

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace session

