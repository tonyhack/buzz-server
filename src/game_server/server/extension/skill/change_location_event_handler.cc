//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-08 15:29:27.
// File name: change_location_event_handler.cc
//
// Description:
// Define class ChangeLocationEventHandler.
//

#include "game_server/server/extension/skill/change_location_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/extension/skill/skill_status.h"

namespace game {

namespace server {

namespace skill {

ChangeLocationEventHandler::ChangeLocationEventHandler() {}
ChangeLocationEventHandler::~ChangeLocationEventHandler() {}

bool ChangeLocationEventHandler::Initialize() {
  this->connection1_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ChangeLocationEventHandler::OnEvent, this, _1, _2, _3, _4));

  this->connection2_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_NPC,
      boost::bind(&ChangeLocationEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ChangeLocationEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection1_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection2_);
}

void ChangeLocationEventHandler::OnEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(channel_type != event::ChannelType::CHANNEL_ACTOR &&
      channel_type != event::ChannelType::CHANNEL_NPC) {
    global::LogError("%s:%d (%s) 参数 channel_type(%u) 错误",
        __FILE__, __LINE__, __FUNCTION__, channel_type);
    return ;
  }

  event::EventSceneRoleChangeLocation *event =
    (event::EventSceneRoleChangeLocation *)message;

  SkillRole *role = NULL;
  role = SkillRoleManager::GetInstance()->Get(event->type_, event->id_);
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

  status->OnEvent(event::EventType::EVENT_SCENE_ROLE_CHANGE_LOCATION,
      message, size);
}

}  // namespace skill

}  // namespace server

}  // namespace game

