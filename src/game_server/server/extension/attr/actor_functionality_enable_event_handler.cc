#include "game_server/server/extension/attr/actor_functionality_enable_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "entity/functionality_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"

namespace game {

namespace server {

namespace attr {

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
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if (NULL == actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->actor_id_);
    return;
  }

  // 属性扩展是否已存在
  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", event->actor_id_);
    return;
  }

  if (entity::FunctionalityType::SWORD_MODULE == event->functionality_id_) {
    if (attr_actor->GetSwordActor()->EnableSwordFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable SPIRIT_WING_FACADE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::NOBLE_MODULE == event->functionality_id_) {
    if (attr_actor->GetNobleActor()->EnableNobleFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable NOBLE_MODULE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::ELEMENT_MODULE == event->functionality_id_) {
    if (attr_actor->GetElementActor()->EnableElementFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable ELEMENT_MODULE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::SPIRIT_MODULE == event->functionality_id_) {
    if (attr_actor->GetSpiritActor()->EnableHorseFacadeFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable SPIRIT_MODULE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::SPIRIT_WING_FACADE == event->functionality_id_) {
    if (attr_actor->GetSpiritActor()->EnableWingFacadeFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable SPIRIT_WING_FACADE failed.", event->actor_id_);
      return;
    }
  } else if (entity::FunctionalityType::LEGENDARY_WEAPON_MODULE == event->functionality_id_) {
    if (attr_actor->GetLegendaryWeaponActor()->EnableLegendaryWeaponFunc() == false) {
      LOG_ERROR("Actor(%lu) Enable LEGENDARY_WEAPON_MODULE failed.", event->actor_id_);
      return;
    }
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

