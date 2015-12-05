#include "game_server/server/extension/scene/fighting_status_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension/scene/scene_actor.h"
#include "game_server/server/extension/scene/scene_extension.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace scene {

FightingStatusEventHandler::FightingStatusEventHandler() {}
FightingStatusEventHandler::~FightingStatusEventHandler() {}

bool FightingStatusEventHandler::Initialize() {
  this->conn_enter_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_ENTER_PVP_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&FightingStatusEventHandler::OnEventEnterPVPFightingStatus,
        this, _1, _2, _3, _4));

  this->conn_leave_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ROLE_LEAVE_PVP_STATUS, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&FightingStatusEventHandler::OnEventLeavePVPFightingStatus,
        this, _1, _2, _3, _4));

  return true;
}

void FightingStatusEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_enter_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_leave_);
}

void FightingStatusEventHandler::OnEventEnterPVPFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleEnterPvpStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleEnterPvpStatus *event =
    (event::EventRoleEnterPvpStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  SceneActor *scene_actor = SceneExtension::GetInstance()->GetSceneActor(event->id_);
  if(scene_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->id_);
    return ;
  }

  scene_actor->SetPVPFightingStatus(true);
}

void FightingStatusEventHandler::OnEventLeavePVPFightingStatus(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventRoleLeavePvpStatus)) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventRoleLeavePvpStatus *event =
    (event::EventRoleLeavePvpStatus *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  SceneActor *scene_actor = SceneExtension::GetInstance()->GetSceneActor(event->id_);
  if(scene_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->id_);
    return ;
  }

  scene_actor->SetPVPFightingStatus(false);
}

}  // namespace scene

}  // namespace server

}  // namespace game

