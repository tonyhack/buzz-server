//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-24 16:42:35.
// File name: actor_compare_event_handler.cc
//
// Description:
// Define class ActorCompareEventHandler.
//

#include "game_server/server/extension/attr/actor_compare_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/attr_configure.h"
#include "game_server/server/extension/attr/configure.h"
#include "game_server/server/extension/attr/sword_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace attr {

ActorCompareEventHandler::ActorCompareEventHandler() {}
ActorCompareEventHandler::~ActorCompareEventHandler() {}

bool ActorCompareEventHandler::Initialize() {
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_COMPARE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorCompareEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorCompareEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void ActorCompareEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventActorCompare)) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorCompare *event = (event::EventActorCompare *)message;

  AttrActor *compare_attr_actor = AttrActorManager::GetInstance()->Get(event->compare_actor_);
  if(compare_attr_actor == NULL) {
    LOG_ERROR("Get AttrActor(%lu) failed.", event->compare_actor_);
    return;
  }

  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->actor_);

  this->OnSwordCompare(compare_attr_actor, attr_actor, event->actor_);
  this->OnNobleCompare(compare_attr_actor, attr_actor, event->actor_);
  this->OnElementCompare(compare_attr_actor, attr_actor, event->actor_);
  this->OnSpiritCompare(compare_attr_actor, attr_actor, event->actor_);
}

void ActorCompareEventHandler::OnSwordCompare(AttrActor *compare_attr_actor,
    AttrActor *attr_actor, core::uint64 actor) {
  SwordActor *compare_sword_actor = compare_attr_actor->GetSwordActor();
  if (compare_sword_actor == NULL) {
    LOG_ERROR("Get SwordActor(%lu) failed.", compare_attr_actor->GetID());
    return;
  }

  gateway::protocol::MessageAttrSwordCompareResponse response;
  response.__set_actor_id_(compare_attr_actor->GetStringID());
  response.__set_step_(compare_sword_actor->GetAvailStep());
  response.__set_level_(compare_sword_actor->GetLevel());

  if (attr_actor) {
    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_RESPONSE);
  } else {
    core::uint64 actors[1] = { actor };
    GameServerSingleton::GetInstance().MulticastMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_SWORD_COMPARE_RESPONSE, actors, 1);
  }
}

void ActorCompareEventHandler::OnNobleCompare(AttrActor *compare_attr_actor,
    AttrActor *attr_actor, core::uint64 actor) {
  NobleActor *compare_noble_actor = compare_attr_actor->GetNobleActor();
  if (compare_noble_actor == NULL) {
    LOG_ERROR("Get NobleActor(%lu) failed.", compare_attr_actor->GetID());
    return;
  }

  gateway::protocol::MessageAttrNobleCompareResponse response;
  response.__set_actor_id_(compare_attr_actor->GetStringID());
  response.__set_result_(true);
  response.__set_level_(compare_noble_actor->GetCurrentLevel());
  response.__set_max_hp_add_value_(compare_noble_actor->GetMaxHpAddValue());
  response.__set_max_mp_add_value_(compare_noble_actor->GetMaxMpAddValue());
  response.__set_physics_attack_add_value_(compare_noble_actor->GetPhysicsAttackAddValue());
  response.__set_physics_defence_add_value_(compare_noble_actor->GetPhysicsDefenceAddValue());
  response.__set_magic_attack_add_value_(compare_noble_actor->GetMagicAttackAddValue());
  response.__set_magic_defence_add_value_(compare_noble_actor->GetMagicDefenceAddValue());
  response.__set_dodge_add_value_(compare_noble_actor->GetDodgeAddValue());
  response.__set_crit_add_value_(compare_noble_actor->GetCritAddValue());
  response.__set_ignore_defence_add_value_(compare_noble_actor->GetIgnoreDefenceAddValue());

  if(attr_actor) {
    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE);
  } else {
    core::uint64 actors[1] = { actor };
    GameServerSingleton::GetInstance().MulticastMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_NOBLE_COMPARE_RESPONSE, actors, 1);
  }
}

void ActorCompareEventHandler::OnElementCompare(AttrActor *compare_attr_actor,
    AttrActor *attr_actor, core::uint64 actor) {
  ElementActor *compare_element_actor = compare_attr_actor->GetElementActor();
  if (compare_element_actor == NULL) {
    LOG_ERROR("Get ElementActor(%lu) failed.", compare_attr_actor->GetID());
    return;
  }

  gateway::protocol::MessageAttrElementCompareResponse response;
  response.__set_actor_id_(compare_attr_actor->GetStringID());

  if (compare_element_actor->GetCurrentLevel() == 0) {
    response.__set_result_(true);
    response.__set_level_(0);
    response.__set_max_hp_add_value_(0);
    response.__set_max_mp_add_value_(0);
    response.__set_physics_attack_add_value_(0);
    response.__set_physics_defence_add_value_(0);
    response.__set_magic_attack_add_value_(0);
    response.__set_magic_defence_add_value_(0);
    response.__set_dodge_add_value_(0);
    response.__set_crit_add_value_(0);
    response.__set_ignore_defence_add_value_(0);
  } else {
    const ElementCell *cell = Configure::GetInstance()->GetElement(
        compare_element_actor->GetCurrentLevel());
    if (NULL == cell) {
      LOG_ERROR("ElementCell(%d) not found in config.",
          compare_element_actor->GetCurrentLevel());
      return;
    }

    response.__set_result_(true);
    response.__set_level_(compare_element_actor->GetCurrentLevel());
    response.__set_max_hp_add_value_(cell->max_hp_add_value_);
    response.__set_max_mp_add_value_(cell->max_mp_add_value_);
    response.__set_physics_attack_add_value_(cell->physics_attack_add_value_);
    response.__set_physics_defence_add_value_(cell->physics_defence_add_value_);
    response.__set_magic_attack_add_value_(cell->magic_attack_add_value_);
    response.__set_magic_defence_add_value_(cell->magic_defence_add_value_);
    response.__set_dodge_add_value_(cell->dodge_add_value_);
    response.__set_crit_add_value_(cell->crit_add_value_);
    response.__set_ignore_defence_add_value_(cell->ignore_defence_add_value_);
  }

  if(attr_actor) {
    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE);
  } else {
    core::uint64 actors[1] = { actor };
    GameServerSingleton::GetInstance().MulticastMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_ELEMENT_COMPARE_RESPONSE, actors, 1);
  }
}

void ActorCompareEventHandler::OnSpiritCompare(AttrActor *compare_attr_actor,
    AttrActor *attr_actor, core::uint64 actor) {
  SpiritActor *compare_spirit_actor = compare_attr_actor->GetSpiritActor();
  if (compare_spirit_actor == NULL) {
    LOG_ERROR("Get SpiritActor(%lu) failed.", compare_attr_actor->GetID());
    return;
  }

  RuneActor *rune_actor = compare_attr_actor->GetRuneActor();
  if (rune_actor == NULL) {
    LOG_ERROR("Get RuneActor(%lu) failed.", compare_attr_actor->GetID());
    return;
  }

  gateway::protocol::MessageAttrSpiritCompareResponse response;
  response.__set_result_(true);
  response.__set_actor_id_(compare_attr_actor->GetStringID());
  response.__set_level_(compare_spirit_actor->GetCurrentLevel());
  response.__set_current_facade_type_((entity::SpiritFacadeType::type)compare_spirit_actor->GetCurrentFacadeType());
  response.__set_horse_facade_id_(compare_spirit_actor->GetHorseFacadeID());
  response.__set_wing_facade_id_(compare_spirit_actor->GetWingFacadeID());

  std::vector<gateway::protocol::SpiritRuneData> val;
  rune_actor->GetRuneSlots(val);
  response.__set_runes_(val);

  for (size_t i = 0; i < compare_spirit_actor->GetFacades().size(); ++i) {
    const SpiritFacade &facade = compare_spirit_actor->GetFacades()[i];
    if (facade.CheckExist() == false) {
      continue;
    }

    gateway::protocol::SpiritFacadeData data;
    data.__set_id_(facade.GetID());
    data.__set_expire_time_(facade.GetExpireTime());

    response.facades_.push_back(data);
  }
  response.__isset.facades_ = true;

  if(attr_actor) {
    attr_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE);
  } else {
    core::uint64 actors[1] = { actor };
    GameServerSingleton::GetInstance().MulticastMessage(response,
        gateway::protocol::MessageType::MESSAGE_ATTR_SPIRIT_COMPARE_RESPONSE, actors, 1);
  }
}

}  // namespace attr

}  // namespace server

}  // namespace game

