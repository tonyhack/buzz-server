//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 14:30:10.
// File name: scene_role_create_event_handler.cc
//
// Description:
// Define class SceneRoleCreateEventHandler.
//

#include "game_server/server/extension/team/scene_role_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/team/team_actor.h"
#include "game_server/server/extension/team/team_actor_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_team_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace team {

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

void SceneRoleCreateEventHandler::OnSceneBroadcastCreateRoleEvent(
    core::uint64 channel, core::uint8 channel_type, const void *data,
    size_t size) {
  if(data == NULL || sizeof(event::EventSceneBroadcastCreateRole) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 data 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)data;

  // 非玩家对象不处理
  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->actors_.size() == 0) {
    return ;
  }

  TeamActor *actor = TeamActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    LOG_ERROR("获取 TeamActor(%lu) 失败", event->id_);
    return ;
  }

  gateway::protocol::ActorTeamFields field;
  field.__set_actor_(global::ToString(event->id_));
  field.__set_team_(global::ToString(actor->GetTeam()));

  gateway::protocol::MessageTeamActorsBroadcast broadcast;
  broadcast.actors_.push_back(field);

  GameServerSingleton::GetInstance().MulticastMessage(broadcast,
      gateway::protocol::MessageType::MESSAGE_TEAM_ACTORS_BROADCAST,
      (core::uint64 *)&event->actors_[0], event->actors_.size());
}

void SceneRoleCreateEventHandler::OnSceneCreateAoiRolesEvent(core::uint64 channel,
    core::uint8 channel_type, const void *data, size_t size) {
  if(data == NULL || sizeof(event::EventSceneCreateAoiRoles) != size ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    global::LogError("%s:%d (%s) 参数 data 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSceneCreateAoiRoles *event = (event::EventSceneCreateAoiRoles *)data;

  if(event->roles_.size() == 0) {
    return ;
  }

  // 获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor对象(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  gateway::protocol::ActorTeamFields field;
  gateway::protocol::MessageTeamActorsBroadcast message;

  size_t pos = 0;
  for(; pos < event->roles_.size(); ++pos) {
    if(event->roles_[pos].type_ == entity::EntityType::TYPE_ACTOR) {
      TeamActor *team_actor = TeamActorManager::GetInstance()->Get(
          event->roles_[pos].id_);
      if(team_actor == NULL) {
        LOG_ERROR("获取 TeamActor(%lu) 失败", event->roles_[pos].id_);
        continue;
      }
      if(team_actor->GetTeam() != 0) {
        field.__set_actor_(global::ToString(team_actor->GetID()));
        field.__set_team_(global::ToString(team_actor->GetTeam()));
        message.actors_.push_back(field);
        if(message.actors_.size() > kMaxCreateEquipSize_) {
          actor->SendMessage(message,
              gateway::protocol::MessageType::MESSAGE_TEAM_ACTORS_BROADCAST);
          message.actors_.clear();
        }
      }
    }
  }

  if(message.actors_.empty() == false) {
    actor->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_TEAM_ACTORS_BROADCAST);
  }
}

}  // namespace team

}  // namespace server

}  // namespace game

