//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-09 12:07:17.
// File name: request_clear_npc.cc
//
// Description:
// Define class RequestClearNpc.
//

#include "game_server/server/extension/scene/request_clear_npc.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/extension/scene/scene_npc_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestClearNpc::RequestClearNpc() {}
RequestClearNpc::~RequestClearNpc() {}

bool RequestClearNpc::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_CLEAR_NPC,
      boost::bind(&RequestClearNpc::OnRequest, this, _1, _2));

  return true;
}

void RequestClearNpc::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_CLEAR_NPC);
}

int RequestClearNpc::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneClearNpc) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneClearNpc *request = (request::RequestSceneClearNpc *)message;

  SceneNpc *npc = SceneNpcManager::GetInstance()->Get(request->id_);
  if(npc == NULL) {
    LOG_ERROR("获取 SceneNpc(%lu) 失败", request->id_);
    return -1;
  }

  Scene *scene = npc->GetScene();
  if(scene == NULL) {
    LOG_ERROR("获取 Scene 失败");
    return -1;
  }

  if(request->__isset.delay_secs_ >= 0) {
    scene->GetEntityBuilder().DisappearNpc(request->id_, request->delay_secs_);
  } else {
    scene->GetEntityBuilder().DisappearNpc(request->id_, 0);
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game
