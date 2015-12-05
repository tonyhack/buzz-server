#include "session_server/server/extension/social/actor_change_vocation_event_handler.h"

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

ActorChangeVocationEventHandler::ActorChangeVocationEventHandler() {}
ActorChangeVocationEventHandler::~ActorChangeVocationEventHandler() {}

bool ActorChangeVocationEventHandler::Initialize() {
  // Follow 玩家切换职业事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_CHANGE_VOCATION, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorChangeVocationEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorChangeVocationEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorChangeVocationEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorChangeVocation) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorChangeVocation *receive_event = (event::EventActorChangeVocation *)message;
  // 获取公会玩家对象
  SocialActor *social_actor =
      SocialActorManager::GetInstance()->Get(receive_event->actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.",
              receive_event->actor_id_);
    return;
  }

  // 发送信息更新事件
  {
    event::EventSocialContactsInfoUpdate event;
    event.__set_actor_id_(receive_event->actor_id_);
    event.__set_vocation_(social_actor->GetVocation());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_CONTACTS_INFO_UPDATE, receive_event->actor_id_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
  {
    event::EventSocialEnemyInfoUpdate event; 
    event.__set_actor_id_(receive_event->actor_id_);
    event.__set_vocation_(social_actor->GetVocation());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_SOCIAL_ENEMY_INFO_UPDATE, receive_event->actor_id_,
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

