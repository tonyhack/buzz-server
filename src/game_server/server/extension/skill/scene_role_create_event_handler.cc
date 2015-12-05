//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-20 16:13:38.
// File name: scene_role_create_event_handler.cc
//
// Description:
// Define class SceneRoleCreateEventHandler.
//

#include "game_server/server/extension/skill/scene_role_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"
#include "global/common_functions.h"

namespace game {

namespace server {

namespace skill {

SceneRoleCreateEventHandler::SceneRoleCreateEventHandler() {}
SceneRoleCreateEventHandler::~SceneRoleCreateEventHandler() {}

bool SceneRoleCreateEventHandler::Initialize() {
  // Follow 所有场景广播创建角色事件
  this->conn_scene_broadcast_create_role_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_BROADCAST_CREATE_ROLE,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_SCENE, 
      boost::bind(&SceneRoleCreateEventHandler::OnSceneBroadcastCreateRoleEvent,
        this, _1, _2, _3, _4));

  // Follow 所有场景创建视野内角色列表事件
  this->conn_scene_broadcast_create_role_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_CREATE_AOI_ROLES,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_SCENE, 
      boost::bind(&SceneRoleCreateEventHandler::OnSceneCreateAoiRolesEvent,
        this, _1, _2, _3, _4));

  return true;
}

void SceneRoleCreateEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_scene_broadcast_create_role_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(
      this->conn_scene_create_aoi_roles_);
}

void SceneRoleCreateEventHandler::OnSceneBroadcastCreateRoleEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneBroadcastCreateRole) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->actors_.size() == 0) {
    return;
  }

  SkillRole *skill_role = SkillRoleManager::GetInstance()->GetActor(event->id_);
  if(skill_role == NULL) {
    LOG_ERROR("获取 SkillRole(%lu) 失败", event->id_);
    return;
  }

  if(skill_role->GetFightingStatus().GetPvpStatus() == true) {
    gateway::protocol::MessagePvpStatusBroadcast broadcast;
    broadcast.actors_.push_back(global::ToString(event->id_));
    GameServerSingleton::GetInstance().MulticastMessage(broadcast,
        gateway::protocol::MessageType::MESSAGE_PVP_STATUS_BROADCAST,
        (core::uint64 *)&event->actors_[0], event->actors_.size());
  }
}

void SceneRoleCreateEventHandler::OnSceneCreateAoiRolesEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneCreateAoiRoles) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("参数 message 或 size 或 channel_type 错误");
    return ;
  }

  event::EventSceneCreateAoiRoles *event = (event::EventSceneCreateAoiRoles *)message;

  if(event->roles_.size() == 0) {
    return ;
  }

  // 获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_);
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor对象(%lu) 失败", event->actor_);
    return ;
  }

  gateway::protocol::MessagePvpStatusBroadcast broadcast;

  size_t pos = 0;
  for(; pos < event->roles_.size(); ++pos) {
    if(event->roles_[pos].type_ == entity::EntityType::TYPE_ACTOR) {
      SkillRole *skill_actor = SkillRoleManager::GetInstance()->GetActor(
          event->roles_[pos].id_);
      if(skill_actor == NULL) {
        LOG_ERROR("获取 SkillActor(%lu) 失败", event->roles_[pos].id_);
        continue;
      }
      if(skill_actor->GetFightingStatus().GetPvpStatus() == true) {
        broadcast.actors_.push_back(global::ToString(skill_actor->GetID()));
        if(broadcast.actors_.size() > kMaxCreateSize_) {
          actor->SendMessage(broadcast,
              gateway::protocol::MessageType::MESSAGE_PVP_STATUS_BROADCAST);
          broadcast.actors_.clear();
        }
      }
    }
  }

  if(broadcast.actors_.empty() == false) {
    actor->SendMessage(broadcast,
        gateway::protocol::MessageType::MESSAGE_PVP_STATUS_BROADCAST);
  }
}

}  // namespace skill

}  // namespace server

}  // namespace game

