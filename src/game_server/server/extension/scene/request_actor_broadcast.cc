//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-27 13:45:02.
// File name: request_actor_broadcast.cc
//
// Description:
// Define class RequestActorBroadcast.
//

#include "game_server/server/extension/scene/request_actor_broadcast.h"

#include <boost/bind.hpp>

#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_scene_request_types.h"

namespace game {

namespace server {

namespace scene {

RequestActorBroadcast::RequestActorBroadcast() {}
RequestActorBroadcast::~RequestActorBroadcast() {}

bool RequestActorBroadcast::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_SCENE_ACTOR_BROADCAST,
      boost::bind(&RequestActorBroadcast::OnRequest, this, _1, _2));

  return true;
}

void RequestActorBroadcast::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_SCENE_ACTOR_BROADCAST);
}

int RequestActorBroadcast::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestSceneActorBroadcast) != size) {
    LOG_ERROR("参数错误");
    return -1;
  }

  request::RequestSceneActorBroadcast *request =
    (request::RequestSceneActorBroadcast *)message;

  if(request->type_ < gateway::protocol::MessageType::MESSAGE_MIN ||
      request->type_ >= gateway::protocol::MessageType::MESSAGE_MAX) {
    LOG_ERROR("消息类型(%d)错误", request->type_);
    return -1;
  }

  Scene *scene = SceneManager::GetInstance()->GetScene(request->scene_);
  if(scene == NULL) {
    LOG_ERROR("获取场景对象(%lu) 失败", request->scene_);
    return -1;
  }

  // 获取当前场景中的所有玩家
  const Scene::ActorHashmap &actors = scene->GetActors();

  if(actors.empty() == false) {
    protocol::MessageUserMessageMulticast multicast;
    multicast.__set_type_((gateway::protocol::MessageType::type)request->type_);
    multicast.__set_message_(request->message_);
    Scene::ActorHashmap::const_iterator iterator = actors.begin();
    for(; iterator != actors.end(); ++iterator) {
      multicast.actors_.push_back(iterator->first);
    }
    GameServerSingleton::GetInstance().BroadcastGateway(multicast,
        protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
  }

  return 0;
}

}  // namespace scene

}  // namespace server

}  // namespace game

