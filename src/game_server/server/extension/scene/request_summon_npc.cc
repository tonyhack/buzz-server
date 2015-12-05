//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-22 16:06:55.
// File name: request_summon_npc.cc
//
// Description:
// Define class RequestSummonNpc.
//

#include "game_server/server/extension/scene/request_summon_npc.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

RequestSummonNpc::RequestSummonNpc() {}
RequestSummonNpc::~RequestSummonNpc() {}

bool RequestSummonNpc::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_SUMMON_NPC,
      boost::bind(&RequestSummonNpc::OnRequest, this, _1, _2));
  return true;
}

void RequestSummonNpc::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_SUMMON_NPC);
}

int RequestSummonNpc::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneSummonNpc) != size) {
    global::LogError("%s:%d (%s) 请求召唤NPC时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestSceneSummonNpc *request =
    (request::RequestSceneSummonNpc *)message;

  // 获取 Scene 对象
  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 请求召唤NPC时，获取 Scene 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  // 召唤NPC
  GameNpc *npc = scene->GetEntityBuilder().SummonNpc(
      request->template_id_, request->x_,request->y_,
      request->width_, request->heigth_, request->dir_, request->delay_secs_);
  if(npc == NULL) {
    LOG_ERROR("召唤NPC失败");
    return -1;
  }
  request->__set_npc_id_(npc->GetID());

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game


