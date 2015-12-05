//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-25 10:51:08.
// File name: skill_event_handler.cc
//
// Description:
// Define class SkillEventHandler.
//

#include "game_server/server/extension/buff/skill_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"

namespace game {

namespace server {

namespace buff {

SkillEventHandler::SkillEventHandler() {}
SkillEventHandler::~SkillEventHandler() {}

bool SkillEventHandler::Initialize() {
  this->conn_npc_attack_hit_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_ATTACK_HIT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&SkillEventHandler::OnEventAttackHit,
        this, _1, _2, _3, _4));

  this->conn_actor_attack_hit_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_ATTACK_HIT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SkillEventHandler::OnEventAttackHit,
        this, _1, _2, _3, _4));

  this->conn_npc_attacked_hit_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_ATTACKED_HIT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&SkillEventHandler::OnEventAttackedHit,
        this, _1, _2, _3, _4));

  this->conn_actor_attacked_hit_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_ATTACKED_HIT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SkillEventHandler::OnEventAttackedHit,
        this, _1, _2, _3, _4));

  this->conn_npc_use_skill_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_USE_SKILL, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&SkillEventHandler::OnEventSkillUse,
        this, _1, _2, _3, _4));

  this->conn_actor_use_skill_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_USE_SKILL, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SkillEventHandler::OnEventSkillUse,
        this, _1, _2, _3, _4));

  return true;
}

void SkillEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_attack_hit_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_attack_hit_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_attacked_hit_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_attacked_hit_);

  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_use_skill_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_use_skill_);
}

void SkillEventHandler::OnEventAttackHit(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSkillAttackHit) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSkillAttackHit *event = (event::EventSkillAttackHit *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return;
  }

  role->OnEvent(event::EventType::EVENT_SKILL_ATTACK_HIT, message, size);
}

void SkillEventHandler::OnEventAttackedHit(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSkillAttackedHit) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSkillAttackedHit *event = (event::EventSkillAttackedHit *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return;
  }

  role->OnEvent(event::EventType::EVENT_SKILL_ATTACKED_HIT, message, size);

  role->OnHurt();
}

void SkillEventHandler::OnEventSkillUse(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventUseSkill) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventUseSkill *event = (event::EventUseSkill *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return;
  }

  role->OnEvent(event::EventType::EVENT_SKILL_USE_SKILL, message, size);
}

}  // namespace buff

}  // namespace server

}  // namespace game

