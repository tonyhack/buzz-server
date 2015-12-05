//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-18 13:38:26.
// File name: request_scene_pk_check.cc
//
// Description:
// Define class RequestScenePKCheck.
//

#include "game_server/server/extension/scene/request_scene_pk_check.h"

#include <boost/bind.hpp>
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestScenePKCheck::RequestScenePKCheck() {}
RequestScenePKCheck::~RequestScenePKCheck() {}

bool RequestScenePKCheck::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_PK_CHECK,
      boost::bind(&RequestScenePKCheck::OnRequest, this, _1, _2));
  return true;
}

void RequestScenePKCheck::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_PK_CHECK);
}

int RequestScenePKCheck::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestScenePKCheck) != size) {
    global::LogError("%s:%d (%s) 请求角色移动时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestScenePKCheck *request = (request::RequestScenePKCheck *)message;

  GameActor *src_actor = GameActorManager::GetInstance()->GetActor(request->src_id_);
  if(src_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->src_id_);
    return -1;
  }

  GameActor *dest_actor = GameActorManager::GetInstance()->GetActor(request->dest_id_);
  if(dest_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->dest_id_);
    return -1;
  }

  // 不在一个场景
  if(src_actor->GetScene() != dest_actor->GetScene()) {
    global::LogError("%s:%d (%s) 技能释放者和目标不在一个场景SrcActor(%lu) DestActor(%lu)",
        __FILE__, __LINE__, __FUNCTION__, request->src_id_, request->dest_id_);
    return -1;
  }
 
  // 获取 GameActor 所在 Scene 对象
  Scene *scene = SceneManager::GetInstance()->GetScene(src_actor->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor(%lu) Scene(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, request->src_id_, src_actor->GetScene());
    return -1;
  }

  // 得到检测结果
  request->__set_result_(scene->PKCheck(src_actor, dest_actor));


  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

