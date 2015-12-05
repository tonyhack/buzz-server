//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-08 15:54:46.
// File name: basic_status_change_event_handler.cc
//
// Description:
// Define BasicStatusChangeEventHandler.
//

#include "game_server/server/extension/skill/basic_status_change_event_handler.h"

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

BasicStatusChangeEventHandler::BasicStatusChangeEventHandler() {}
BasicStatusChangeEventHandler::~BasicStatusChangeEventHandler() {}

bool BasicStatusChangeEventHandler::Initialize() {
  // Follow 所有玩家状态转换事件
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&BasicStatusChangeEventHandler::OnActorEvent,
      this, _1, _2, _3, _4));
  // Follow 所有NPC状态转换事件
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&BasicStatusChangeEventHandler::OnNpcEvent,
      this, _1, _2, _3, _4));

  return true;
}
void BasicStatusChangeEventHandler::Finalize() {}

void BasicStatusChangeEventHandler::OnActorEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  SkillRole *role = NULL;
  role = SkillRoleManager::GetInstance()->GetActor(event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，找不到角色(%u,%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  SkillStatus *status = role->GetCurrentStatus();
  if(status == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，技能状态(%u,%lu)错误",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  status->OnEvent(event::EventType::EVENT_BASIC_STATUS_CHANGE,
      message, size);
}

void BasicStatusChangeEventHandler::OnNpcEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size ||
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_NPC) {
    return ;
  }

  SkillRole *role = NULL;
  role = SkillRoleManager::GetInstance()->GetNpc(event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，找不到角色(%u,%lu)",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  SkillStatus *status = role->GetCurrentStatus();
  if(status == NULL) {
    global::LogError("%s:%d (%s) 响应角色位置改变事件时，技能状态(%u,%lu)错误",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  status->OnEvent(event::EventType::EVENT_BASIC_STATUS_CHANGE,
      message, size);
}

}  // namespace skill

}  // namespace server

}  // namespace game

