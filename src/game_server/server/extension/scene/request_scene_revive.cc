//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-16 16:42:14.
// File name: request_scene_revive.cc
//
// Description:
// Define class RequestSceneRevive.
//

#include "game_server/server/extension/scene/request_scene_revive.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestSceneRevive::RequestSceneRevive() {}
RequestSceneRevive::~RequestSceneRevive() {}

bool RequestSceneRevive::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_REVIVE,
      boost::bind(&RequestSceneRevive::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneRevive::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_REVIVE);
}

int RequestSceneRevive::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneRevive) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneRevive *request = (request::RequestSceneRevive *)message;

  GameActor *actor = GameActorManager::GetInstance()->GetActor(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", request->actor_);
    return -1;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    LOG_ERROR("请求召唤NPC时，获取 Scene(%lu) 失败", actor->GetScene());
    return -1;
  }

  SceneActor *scene_actor = scene->GetActor(request->actor_);
  if(scene_actor == NULL) {
    LOG_ERROR("获取 SceneActor(%lu) 失败", request->actor_);
    return -1;
  }

  scene_actor->Revive(request->stay_revive_ == false);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game
