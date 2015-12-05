//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-12 14:37:09.
// File name: request_clear_all_npc.cc
//
// Description:
// Define class RequestClearAllNpc.
//

#include "game_server/server/extension/scene/request_clear_all_npc.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestClearAllNpc::RequestClearAllNpc() {}
RequestClearAllNpc::~RequestClearAllNpc() {}

bool RequestClearAllNpc::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_CLEAR_ALL_NPC,
      boost::bind(&RequestClearAllNpc::OnRequest, this, _1, _2));

  return true;
}

void RequestClearAllNpc::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_CLEAR_ALL_NPC);
}

int RequestClearAllNpc::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneClearAllNpc) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneClearAllNpc *request =
    (request::RequestSceneClearAllNpc *)message;

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    LOG_ERROR("请求召唤NPC时，获取 Scene(%lu) 失败", request->scene_);
    return -1;
  }

  scene->ClearNpcs(request->delay_secs_ < 0 ? 0: request->delay_secs_, request->force_clear_reborn_);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

