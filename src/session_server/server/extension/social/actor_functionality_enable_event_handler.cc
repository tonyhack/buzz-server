#include "session_server/server/extension/social/actor_functionality_enable_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "entity/functionality_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/social/social_actor.h"
#include "session_server/server/extension/social/social_actor_manager.h"

namespace session {

namespace server {

namespace social {

ActorFunctionalityEnableEventHandler::ActorFunctionalityEnableEventHandler() {}
ActorFunctionalityEnableEventHandler::~ActorFunctionalityEnableEventHandler() {}

bool ActorFunctionalityEnableEventHandler::Initialize() {
  // Follow 所有玩家系统功能开启事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorFunctionalityEnableEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorFunctionalityEnableEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorFunctionalityEnableEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorFunctionalityEnable) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorFunctionalityEnable *event = (event::EventActorFunctionalityEnable *)message;

  // 从玩家管理器中获取玩家对象
  SocialActor *social_actor = SocialActorManager::GetInstance()->GetFromAll(event->actor_id_);
  if (NULL == social_actor) {
    LOG_ERROR("Get SocialActor(%lu) from SocialActorManager failed.", event->actor_id_);
    return;
  }

  if (entity::FunctionalityType::CONTACTS_MODULE == event->functionality_id_) {
    if (social_actor->EnableContactsFunc() == false) {
      LOG_ERROR("Actor(%lu) enable CONTACTS_MODULE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_MODULE == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_MODULE failed.", event->actor_id_); 
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_GOLDMINE == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandGoldmineFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_GOLDMINE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_POOL == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandPoolFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_POOL failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_TREE == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandTreeFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_TREE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_TOWER == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandTowerFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_TOWER failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::HOMELAND_STRONGHOLD == event->functionality_id_) {
    if (social_actor->GetHomelandActor()->EnableHomelandStrongholdFunc() == false) {
      LOG_ERROR("Actor(%lu) enable HOMELAND_STRONGHOLD failed.", event->actor_id_);
      return;
    }
  }

  // 保存离线数据
  if (social_actor->CheckOnline() == false) {
    social_actor->Save();
  }
}

}  // namespace social

}  // namespace server

}  // namespace session

