//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-18 12:04:56.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "game_server/server/extension/scene/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace scene {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登出事件
  this->conn_actor_logout_ = 
    coresh::CommunicatorSingleton::GetInstance().Follow(
        event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
        event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLogoutEventHandler::OnEvent,
          this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_logout_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登出失败，无法从 GameActorManager 获取 GameActor",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 从场景管理器中获取场景
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登出失败，无法从 SceneManager 获取 scene(%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->id_, actor->GetScene());
    return ;

  }

  // 登出场景
  if(scene->RemoveEntity(actor) == false) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登出失败，从场景移除失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game

