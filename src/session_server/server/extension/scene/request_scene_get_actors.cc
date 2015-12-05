//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-11 10:59:29.
// File name: request_scene_get_actors.cc
//
// Description:
// Define class RequestSceneGetActors.
//

#include "session_server/server/extension/scene/request_scene_get_actors.h"

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

RequestSceneGetActors::RequestSceneGetActors() {}
RequestSceneGetActors::~RequestSceneGetActors() {}

bool RequestSceneGetActors::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_ACTORS,
      boost::bind(&RequestSceneGetActors::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneGetActors::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_GET_ACTORS);
}

int RequestSceneGetActors::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneGetActors) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneGetActors *request = (request::RequestSceneGetActors *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", request->scene_);
    return -1;
  }

  const Scene::ActorSet &actors = scene->GetActorSet();
  Scene::ActorSet::const_iterator iterator = actors.begin();
  for(; iterator != actors.end(); ++iterator) {
    request->actors_.push_back(*iterator);
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace session

