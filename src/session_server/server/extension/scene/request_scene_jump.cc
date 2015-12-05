//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-23 14:10:50.
// File name: request_scene_jump.cc
//
// Description:
// Define class RequestSceneJump.
//

#include "session_server/server/extension/scene/request_scene_jump.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/protocol/game_scene_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_scene_request_types.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_manager.h"

namespace session {

namespace server {

namespace scene {

RequestSceneJump::RequestSceneJump() {}
RequestSceneJump::~RequestSceneJump() {}

bool RequestSceneJump::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_JUMP,
      boost::bind(&RequestSceneJump::OnRequest, this, _1, _2));

  return true;
}

void RequestSceneJump::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_JUMP);
}

int RequestSceneJump::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneJump) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneJump *request = (request::RequestSceneJump *)message;

  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(request->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 SessionActor对象(%lu) 失败", request->actor_);
    return -1;
  }

  if(request->__isset.map_ == true) {
    request->__set_scene_(request->map_);
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    LOG_ERROR("获取 Scene对象(%lu) 失败", request->scene_);
    return -1;
  }

  protocol::MessageSceneJumpNotice jump_notice;
  jump_notice.__set_actor_(request->actor_);
  jump_notice.__set_scene_(request->scene_);
  jump_notice.__set_x_(request->x_);
  jump_notice.__set_y_(request->y_);

  actor->SendGameMessage(jump_notice,
      protocol::GameMessageType::MESSAGE_SCENE_JUMP_NOTICE);

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace session

