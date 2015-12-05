//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-22 18:47:25.
// File name: attribute_change_event_handler.cc
//
// Description:
// Define class AttributeChangeEventHandler.
//

#include "game_server/server/extension/buff/attribute_change_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"

namespace game {

namespace server {

namespace buff {

AttributeChangeEventHandler::AttributeChangeEventHandler() {}
AttributeChangeEventHandler::~AttributeChangeEventHandler() {}

bool AttributeChangeEventHandler::Initialize() {
  this->conn_npc_change_hp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_HP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeHp,
        this, _1, _2, _3, _4));

  this->conn_actor_change_hp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_HP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeHp,
        this, _1, _2, _3, _4));

  this->conn_npc_change_mp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeMp,
        this, _1, _2, _3, _4));

  this->conn_actor_change_mp_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_MP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeMp,
        this, _1, _2, _3, _4));

  this->conn_npc_change_common_status_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&AttributeChangeEventHandler::OnEventRoleCommonStatusChange,
        this, _1, _2, _3, _4));

  this->conn_actor_change_common_status_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttributeChangeEventHandler::OnEventRoleCommonStatusChange,
        this, _1, _2, _3, _4));

  this->conn_npc_change_current_energy_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeCurrentEnergy,
        this, _1, _2, _3, _4));

  this->conn_actor_change_current_energy_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_CHANGE_CURRENT_ENERGY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&AttributeChangeEventHandler::OnEventRoleChangeCurrentEnergy,
        this, _1, _2, _3, _4));

  return true;
}

void AttributeChangeEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_npc_change_hp_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_actor_change_hp_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_npc_change_mp_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_actor_change_mp_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_npc_change_common_status_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_actor_change_common_status_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_npc_change_current_energy_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_actor_change_current_energy_);
}

void AttributeChangeEventHandler::OnEventRoleChangeHp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeHp) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeHp *event = (event::EventRoleChangeHp *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  role->OnEvent(event::EventType::EVENT_ROLE_CHANGE_HP, message, size);
}

void AttributeChangeEventHandler::OnEventRoleChangeMp(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleChangeMp) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleChangeMp *event = (event::EventRoleChangeMp *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  role->OnEvent(event::EventType::EVENT_ROLE_CHANGE_MP, message, size);
}

void AttributeChangeEventHandler::OnEventRoleCommonStatusChange(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleCommonStatusChange) != size) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleCommonStatusChange *event = (event::EventRoleCommonStatusChange *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  role->OnEvent(event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, message, size);
}

void AttributeChangeEventHandler::OnEventRoleChangeCurrentEnergy(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventRoleChangeCurrentEnergy) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventRoleChangeCurrentEnergy *event = (event::EventRoleChangeCurrentEnergy *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  GameRole *game_role = role->GetRole();
  if (game_role == NULL) {
    LOG_ERROR("获取 GameRole(%d,%lu) 失败", event->type_, event->id_);
    return;
  }

  if (game_role->GetAttribute(entity::RoleClientFields::CURRENT_ENERGY) == 0) {
    role->OnCurrentEnergyDepleted();
  }
}

}  // namespace buff

}  // namespace server

}  // namespace game

