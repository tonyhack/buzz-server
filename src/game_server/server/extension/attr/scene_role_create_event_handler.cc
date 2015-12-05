#include "game_server/server/extension/attr/scene_role_create_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_attr_protocol_types.h"
#include "game_server/server/game_server.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/extension/attr/attr_actor.h"
#include "game_server/server/extension/attr/attr_actor_manager.h"
#include "game_server/server/extension/attr/noble_actor.h"

namespace game {

namespace server {

namespace attr {

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

void SceneRoleCreateEventHandler::OnSceneBroadcastCreateRoleEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSceneBroadcastCreateRole) ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventSceneBroadcastCreateRole *event =
    (event::EventSceneBroadcastCreateRole *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR) {
    return ;
  }

  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->id_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", event->id_);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", event->id_);
    return;
  }
  
  gateway::protocol::MessageAttrAoiSynchronize sync;
  gateway::protocol::AttrAoiData data;
  data.__set_actor_id_(attr_actor->GetStringID());
  data.__set_noble_level_(attr_actor->GetNobleActor()->GetCurrentLevel());
  sync.aoi_data_.push_back(data);
  GameServerSingleton::GetInstance().MulticastMessage(sync,
      gateway::protocol::MessageType::MESSAGE_ATTR_AOI_SYNCHRONIZE,
      (core::uint64 *)&event->actors_[0], event->actors_.size());
}

void SceneRoleCreateEventHandler::OnSceneCreateAoiRolesEvent(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSceneCreateAoiRoles) ||
      channel_type != event::ChannelType::CHANNEL_SCENE) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventSceneCreateAoiRoles *event =
    (event::EventSceneCreateAoiRoles *)message;

  AttrActor *attr_actor = AttrActorManager::GetInstance()->Get(event->actor_);
  if (NULL == attr_actor) {
    LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", event->actor_);
    return;
  }
  GameActor *game_actor = attr_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", event->actor_);
    return;
  }

  gateway::protocol::MessageAttrAoiSynchronize sync;

  for (size_t i = 0; i < event->roles_.size(); ++i) {
    if (event->roles_[i].type_ != entity::EntityType::TYPE_ACTOR) {
      continue;
    }

    AttrActor *target_attr_actor =
      AttrActorManager::GetInstance()->Get(event->roles_[i].id_);
    if (NULL == target_attr_actor) {
      LOG_ERROR("Get AttrActor(%lu) from AttrActorManager failed.", event->roles_[i].id_);
      continue;
    }

    if (target_attr_actor->GetNobleActor()->GetCurrentLevel() >= 0) {
      gateway::protocol::AttrAoiData data;
      data.__set_actor_id_(target_attr_actor->GetStringID());
      data.__set_noble_level_(target_attr_actor->GetNobleActor()->GetCurrentLevel());
      sync.aoi_data_.push_back(data);

      if (sync.aoi_data_.size() >= kMaxCreateSize_) {
        attr_actor->SendMessage(sync,
            gateway::protocol::MessageType::MESSAGE_ATTR_AOI_SYNCHRONIZE);
        sync.aoi_data_.clear();
      }
    }
  }

  attr_actor->SendMessage(sync,
      gateway::protocol::MessageType::MESSAGE_ATTR_AOI_SYNCHRONIZE);
}

}  // namespace attr

}  // namespace server

}  // namespace game

