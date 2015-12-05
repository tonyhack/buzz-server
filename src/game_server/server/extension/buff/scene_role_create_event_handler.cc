//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-28 20:55:39.
// File name: scene_role_create_event_handler.cc
//
// Description:
// Define class SceneRoleCreateEventHandler.
//

#include "game_server/server/extension/buff/scene_role_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/item_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/extension/buff/facade_request.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace buff {

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
  this->conn_scene_create_aoi_roles_ =
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

  if(event->actors_.size() == 0) {
    return ;
  }

  if(event->type_ != entity::EntityType::TYPE_ACTOR &&
      event->type_ != entity::EntityType::TYPE_NPC) {
    return;
  }

  // 获取BUFF角色对象
  BuffRole *role = FacadeRequest::GetInstance()->GetBuffRole(event->type_, event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 获取 BuffRole对象(%u,%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  GameRole *game_role = role->GetRole();
  if(game_role == NULL) {
    global::LogError("%s:%d (%s) 获取 GameRole(%u,%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, event->type_, event->id_);
    return ;
  }

  // 导出个角色BUFF数据
  gateway::protocol::BuffRoleField field;
  field.__set_type_(event->type_);
  field.__set_id_(game_role->GetStringID());
  role->ExportBuffs(field.buffs_);

  // BUFF为空，返回
  if(field.buffs_.empty()) {
    return ;
  }

  gateway::protocol::MessageBuffInitialize message;
  message.roles_.push_back(field);

  // 发送给多个玩家
  GameServerSingleton::GetInstance().MulticastMessage(message,
      gateway::protocol::MessageType::MESSAGE_BUFF_INITIALIZE,
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

  // 玩家BUFF对象
  BuffRole *buff_actor = BuffRoleManager::GetInstance()->GetActor(event->actor_);
  if(buff_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 BuffRole对象(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  gateway::protocol::MessageBuffInitialize message;
  gateway::protocol::BuffRoleField field;

  size_t pos = 0;
  for(; pos < event->roles_.size(); ++pos) {
    BuffRole *role = FacadeRequest::GetInstance()->GetBuffRole(
        event->roles_[pos].type_, event->roles_[pos].id_);
    if(role == NULL) {
      global::LogError("%s:%d (%s) 获取 BuffRole对象(%u,%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, event->roles_[pos].type_,
          event->roles_[pos].id_);
      continue ;
    }
    GameRole *game_role = role->GetRole();
    if(game_role == NULL) {
      global::LogError("%s:%d (%s) 获取 GameRole对象(%u,%lu) 失败",
          __FILE__, __LINE__, __FUNCTION__, event->roles_[pos].type_,
          event->roles_[pos].id_);
      continue ;
    }
    // 导出BUFF信息
    field.buffs_.clear();
    role->ExportBuffs(field.buffs_);
    if(field.buffs_.empty()) {
      continue;
    }
    field.__set_type_(event->roles_[pos].type_);
    field.__set_id_(game_role->GetStringID());
    message.roles_.push_back(field);
    if(message.roles_.size() >= kMaxCreateEquipSize_) {
      actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_BUFF_INITIALIZE);
      message.roles_.clear();
    }
  }

  if(message.roles_.empty() == false) {
    actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_BUFF_INITIALIZE);
  }
}

}  // namespace buff

}  // namespace server

}  // namespace game

