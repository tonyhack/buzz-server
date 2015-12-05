//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-29 20:16:10.
// File name: scene_role_create_event_handler.cc
//
// Description:
// Define SceneRoleCreateEventHandler.
//

#include "game_server/server/extension/item/scene_role_create_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/item_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_server.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/equipment_container.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace item {

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

  // 非玩家对象不处理
  if(event->type_ != entity::EntityType::TYPE_ACTOR || event->actors_.size() == 0) {
    return ;
  }

  // 获取Item玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取 ItemActor对象(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 获取装备容器对象
  EquipmentContainer *equip_container = (EquipmentContainer *)actor->GetContainer(
      entity::ItemContainerType::EQUIP);
  if(equip_container == NULL) {
    global::LogError("%s:%d (%s) 获取 玩家(%lu) EquipmentContainer 失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 构建装备信息
  gateway::protocol::MessageCreateActorEquipBroadcast message;
  message.actors_.resize(1);
  message.actors_[0].__set_actor_id_(actor->GetStringID());

  gateway::protocol::EquipInformation equip;

  for(core::uint16 loc = entity::EquipmentType::MIN;
      loc < entity::EquipmentType::MAX; ++loc) {
    Item *item = equip_container->GetItem(loc);
    if(item && ((Equipment *)item)->CheckExpire() == false) {
      equip.__set_location_((entity::EquipmentType::type)loc);
      equip.__set_template_id_(item->GetTemplateID());
      equip.__set_intensify_(((Equipment *)item)->GetAttribute(
          entity::EquipClientFields::INTENSIFY));
      message.actors_[0].equips_.push_back(equip);
    }
  }

  global::LogDebug("广播 玩家(%lu) [%u]条 装备信息", actor->GetID(),
      message.actors_[0].equips_.size());

  // 发送给多个玩家
  GameServerSingleton::GetInstance().MulticastMessage(message,
      gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_EQUIP_BROADCAST,
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

  gateway::protocol::MessageCreateActorEquipBroadcast message;
  gateway::protocol::EquipInformation equip;
  gateway::protocol::ActorEquipInformation actor_equip;

  size_t pos = 0;
  for(; pos < event->roles_.size(); ++pos) {
    actor_equip.equips_.clear();
    if(event->roles_[pos].type_ == entity::EntityType::TYPE_ACTOR) {
      ItemActor *item_actor = ItemActorManager::GetInstance()->Get(
          event->roles_[pos].id_);
      if(item_actor == NULL) {
        global::LogError("%s:%d (%s) 获取 ItemActor对象(%lu) 失败",
            __FILE__, __LINE__, __FUNCTION__, event->roles_[pos].id_);
        continue ;
      }
      // 获取装备容器
      EquipmentContainer *equip_container = (EquipmentContainer *)item_actor->GetContainer(
          entity::ItemContainerType::EQUIP);
      if(equip_container == NULL) {
        global::LogError("%s:%d (%s) 获取 玩家(%lu) EquipmentContainer 失败",
            __FILE__, __LINE__, __FUNCTION__, event->roles_[pos].id_);
        return ;
      }
      for(core::uint16 loc = entity::EquipmentType::MIN;
          loc < entity::EquipmentType::MAX; ++loc) {
        Item *item = equip_container->GetItem(loc);
        if(item && ((Equipment *)item)->CheckExpire() == false) {
          equip.__set_location_((entity::EquipmentType::type)loc);
          equip.__set_template_id_(item->GetTemplateID());
          equip.__set_intensify_(((Equipment *)item)->GetAttribute(
                entity::EquipClientFields::INTENSIFY));
          actor_equip.equips_.push_back(equip);
        }
      }
      if(actor_equip.equips_.size() > 0) {
        // 玩家ID
        actor_equip.__set_actor_id_(item_actor->GetStringID());
        // 压入一个玩家的装备信息
        message.actors_.push_back(actor_equip);
      }
      if(message.actors_.size() >= SceneRoleCreateEventHandler::kMaxCreateEquipSize_) {
        actor->SendMessage(message,
            gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_EQUIP_BROADCAST);
        message.actors_.clear();
      }
    }
  }

  if(message.actors_.empty() == false) {
    actor->SendMessage(message,
        gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR_EQUIP_BROADCAST);
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

