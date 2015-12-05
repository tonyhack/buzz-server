//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-13 02:22:59.
// File name: role_common_status_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/skill/role_common_status_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/extension/skill/skill_status.h"

namespace game {

namespace server {

namespace skill {

RoleCommonStatusEventHandler::RoleCommonStatusEventHandler() {}
RoleCommonStatusEventHandler::~RoleCommonStatusEventHandler() {}

bool RoleCommonStatusEventHandler::Initialize() {
  // Follow 所有角色 commonStatus 状态转换事件
  this->conn_npc_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&RoleCommonStatusEventHandler::OnRoleEvent,
      this, _1, _2, _3, _4));
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleCommonStatusEventHandler::OnRoleEvent,
      this, _1, _2, _3, _4));
  return true;
}
void RoleCommonStatusEventHandler::Finalize() {}

void RoleCommonStatusEventHandler::OnRoleEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventRoleCommonStatusChange) != size) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventRoleCommonStatusChange *event = 
    (event::EventRoleCommonStatusChange *)message;

  SkillRole *role = NULL;
  if(event->type_ == entity::EntityType::TYPE_ACTOR) {
    role = SkillRoleManager::GetInstance()->GetActor(event->id_);
  }
  if(event->type_ == entity::EntityType::TYPE_NPC) {
    role = SkillRoleManager::GetInstance()->GetNpc(event->id_);
  }
  if(role == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，找不到角色(%u,%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  // 获取玩家属性

  SkillStatus *status = role->GetCurrentStatus();
  if(status == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，技能状态(%u,%lu)错误",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  status->OnEvent(event::EventType::EVENT_ROLE_COMMON_STATUS_CHANGE,
      message, size);
}

}  // namespace skill

}  // namespace server

}  // namespace game

