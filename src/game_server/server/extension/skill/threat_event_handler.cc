//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-17 16:55:11.
// File name: threat_event_handler.cc
//
// Description:
// Define class ThreatEventHandler.
//

#include "game_server/server/extension/skill/threat_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_ai_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"

namespace game {

namespace server {

namespace skill {

ThreatEventHandler::ThreatEventHandler() {}
ThreatEventHandler::~ThreatEventHandler() {}

bool ThreatEventHandler::Initialize() {
  this->conn_threat_start_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_AI_THREAT_START, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ThreatEventHandler::OnEventAiThreatStart,
      this, _1, _2, _3, _4));

  this->conn_threat_end_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_AI_THREAT_END, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ThreatEventHandler::OnEventAiThreatEnd,
      this, _1, _2, _3, _4));

  return true;
}

void ThreatEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_threat_start_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_threat_end_);
}

void ThreatEventHandler::OnEventAiThreatStart(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAiThreatStart) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventAiThreatStart *event = (event::EventAiThreatStart *)message;

  // 获取 SkillRole 
  SkillRole *role = SkillRoleManager::GetInstance()->Get(event->threat_role_type_,
      event->threat_role_id_);
  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", event->threat_role_type_,
        event->threat_role_id_);
    return ;
  }

  role->GetFightingStatus().SetStatus();

  // LOG_INFO("玩家(%lu) 进入npc(%lu) 仇恨列表", event->threat_role_id_, event->npc_);
}

void ThreatEventHandler::OnEventAiThreatEnd(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventAiThreatEnd) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventAiThreatEnd *event = (event::EventAiThreatEnd *)message;

  // 获取 SkillRole 
  SkillRole *role = SkillRoleManager::GetInstance()->Get(event->threat_role_type_,
      event->threat_role_id_);
  if(role == NULL) {
    LOG_ERROR("获取 SkillRole(%d,%lu) 失败", event->threat_role_type_,
        event->threat_role_id_);
    return ;
  }

  role->GetFightingStatus().ResetStatus();

  // LOG_INFO("玩家(%lu) 离开npc(%lu) 仇恨列表", event->threat_role_id_, event->npc_);
}

}  // namespace skill

}  // namespace server

}  // namespace game

