//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-15 17:56:22.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "game_server/server/extension/scene/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/channel.h"
#include "global/game_misc.h"

namespace game {

namespace server {

namespace scene {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_actor_login_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_LOGIN,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_login_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登陆失败，无法从 GameActorManager 获取 GameActor",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 从场景管理器中获取场景
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    global::LogError("%s:%d (%s) 玩家(%llu) 登陆失败，无法从 SceneManager 获取 scene(%d)",
        __FILE__, __LINE__, __FUNCTION__, event->id_, actor->GetScene());
    return ;

  }

  gateway::protocol::MessageSceneDayNightChange daynight_message; 
  daynight_message.__set_status_(
      static_cast<gateway::protocol::DayNightStatus::type>(scene->GetSceneDayNight().GetCurStatus()));
  // 登陆后发送昼夜信息
  actor->SendMessage(daynight_message,
      gateway::protocol::MessageType::MESSAGE_SCENE_DAYNIGHT_CHANGE);

  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(event->id_);
  load_finish.__set_extension_type_(ExtensionType::SCENE);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, event->id_,
      event::ChannelType::CHANNEL_ACTOR, &load_finish,
      sizeof(event::EventActorExtensionLoadFinish));

  global::LogDebug("玩家(%lu, %s) 场景扩展加载成功",
      actor->GetID(), actor->GetName().c_str());
}

}  // namespace scene

}  // namespace server

}  // namespace game

