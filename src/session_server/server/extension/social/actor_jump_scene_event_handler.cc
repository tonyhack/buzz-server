#include "session_server/server/extension/social/actor_jump_scene_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/event/session_social_event_types.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ActorJumpSceneEventHandler::ActorJumpSceneEventHandler() {}
ActorJumpSceneEventHandler::~ActorJumpSceneEventHandler() {}

bool ActorJumpSceneEventHandler::Initialize() {
  // Follow 玩家跳场景事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_JUMP_SCENE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorJumpSceneEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorJumpSceneEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorJumpSceneEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorJumpScene) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorJumpScene *receive_event = (event::EventActorJumpScene *)message;
  // 获取公会玩家对象
  SocialActor *social_actor =
      SocialActorManager::GetInstance()->Get(receive_event->actor_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.",
              receive_event->actor_);
    return;
  }

  // 发送信息更新事件
  {
    event::EventSocialContactsInfoUpdate event;
    event.__set_actor_id_(receive_event->actor_);
    event.__set_map_(social_actor->GetMap());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, receive_event->actor_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  {
    event::EventSocialEnemyInfoUpdate event; 
    event.__set_actor_id_(receive_event->actor_);
    event.__set_map_(social_actor->GetMap());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, receive_event->actor_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

