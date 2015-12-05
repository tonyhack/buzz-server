//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-27 19:33:36.
// File name: request_router.cc
//
// Description:
// Define class RequestRouter.
//
#include "game_server/server/extension/scene/request_router.h"

#include <boost/bind.hpp>

#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/request_select_targets.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

RequestRouter::RequestRouter() {}
RequestRouter::~RequestRouter() {}

bool RequestRouter::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_ROUTER,
      boost::bind(&RequestRouter::OnRequest, this, _1, _2));

  return true;
}

void RequestRouter::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_ROUTER);
}

int RequestRouter::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneRouter) != size) {
    global::LogError("%s:%d (%s) 请求寻路时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneRouter *request = (request::RequestSceneRouter *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求寻路时，获取场景(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->scene_);
    return -1;
  }

  RouterCoord src;
  src.x_ = request->src_x_;
  src.y_ = request->src_y_;

  RouterCoord dest;
  dest.x_ = request->dest_x_;
  dest.y_ = request->dest_y_;

  // 寻路
  if(scene->Router(src, dest, request->max_path_, request->dest_radius_) == false) {
    return -1;
  }

  size_t number = 0;

  const entity::DirectionType::type *paths = scene->GetRouterPaths(number);
  if(paths == NULL) {
    global::LogError("%s:%d (%s) 请求寻路(%u,%u->%u,%u)时，场景(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->src_x_, request->src_y_,
        request->dest_x_, request->dest_y_, request->scene_);
    return -1;
  }

  if(scene->CheckReverse() == false) {
    for(size_t pos = 0; pos < number; ++pos) {
      request->paths_.push_back(paths[pos]);
    }
  } else {
    for(size_t pos = 0; pos < number; ++pos) {
      request->paths_.push_back(scene->ReverseDirection(paths[number - pos - 1]));
    }
  }

  request->__set_result_(true);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

