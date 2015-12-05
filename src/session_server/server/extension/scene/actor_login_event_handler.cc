//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-06 17:21:02.
// File name: actor_login_event_handler.cc
//
// Description:
// Define class ActorLoginEventHandler.
//

#include "session_server/server/extension/scene/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "global/logging.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/scene/scene.h"
#include "session_server/server/extension/scene/scene_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"

namespace session {

namespace server {

namespace scene {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLoginEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogin) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 或 channel_type 错误");
    return ;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *actor = SessionActorManager::GetInstance()->GetActor(event->id_);
  if(actor == NULL) {
    LOG_ERROR("参数 获取玩家对象(%lu) 失败");
    return ;
  }

  // 获取玩家所在场景
  Scene *scene = SceneManager::GetInstance()->GetScene(actor->GetScene());
  if(scene == NULL) {
    LOG_ERROR("场景 Scene(%lu) 对象不存在", actor->GetScene());
    return ;
  }

  // 玩家加入到场景玩家列表
  if(scene->AddActor(event->id_) == false) {
    LOG_ERROR("玩家(%lu) 加入场景(%lu) 玩家列表失败", event->id_, actor->GetScene());
  }

  gateway::protocol::MessageSceneWorldBossInitialize msg;
  const SceneManager::SceneHashmap &scenes = SceneManager::GetInstance()->GetScenes();
  for(SceneManager::SceneHashmap::const_iterator itr = scenes.begin();
      itr != scenes.end(); ++itr) {
    if(itr->second && !itr->second->CheckDynamic() && 
          itr->second->GetWorldBoss().GetTemplateID() != 0) {
      gateway::protocol::SceneBossField field;
      field.__set_killer_(itr->second->GetWorldBoss().GetKiller());
      field.__set_template_id_(itr->second->GetWorldBoss().GetTemplateID());
      field.__set_boss_live_(itr->second->GetWorldBoss().GetLive());
      msg.fields_.push_back(field);
    }
  }
  actor->SendMessage(msg,
      gateway::protocol::MessageType::MESSAGE_SCENE_WORLD_BOSS_INITIALIZE);
}

}  // namespace scene

}  // namespace server

}  // namespace session

