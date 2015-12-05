//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 09:31:10.
// File name: request_scene_create_map.cc
//
// Description:
// Define class RequestSceneCreateMap.
//

#include "game_server/server/extension/scene/request_scene_create_map.h"

#include <boost/bind.hpp>

#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_builder.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneCreateMap::RequestSceneCreateMap() {}
RequestSceneCreateMap::~RequestSceneCreateMap() {}

bool RequestSceneCreateMap::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_CREATE_MAP,
      boost::bind(&RequestSceneCreateMap::OnRequestCreateMap, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_DESTORY_MAP,
      boost::bind(&RequestSceneCreateMap::OnRequestDestoryMap, this, _1, _2));

  return true;
}

void RequestSceneCreateMap::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_CREATE_MAP);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_DESTORY_MAP);
}

int RequestSceneCreateMap::OnRequestCreateMap(void *data, size_t size) {
  if(data == NULL || sizeof(request::RequestSceneCreateMap) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneCreateMap *request =
    (request::RequestSceneCreateMap *)data;

  Scene *scene = SceneBuilder::GetInstance()->CreateDynamic(request->map_);
  if(scene == NULL) {
    LOG_DEBUG("创建 Scene对象 失败，地图(%d)", request->map_);
    return -1;
  }

  request->__set_scene_(scene->GetSceneID());

  LOG_DEBUG("动态场景(map=%d scene=%lu) 建立成功", request->map_, scene->GetSceneID());

  return 0;
}

int RequestSceneCreateMap::OnRequestDestoryMap(void *data, size_t size) {
  if(data == NULL || sizeof(request::RequestSceneDestoryMap) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneDestoryMap *request =
    (request::RequestSceneDestoryMap *)data;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    LOG_ERROR("获取 Scene(%lu) 失败", request->scene_);
    return -1;
  }

  SceneBuilder::GetInstance()->Destory(scene);

  LOG_DEBUG("请求销毁场景(%lu) 成功", request->scene_);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

