//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-24 16:04:17.
// File name: session_message_handler.cc
//
// Description:
// Define class SessionMessageHandler.
//

#include "game_server/server/extension/item/session_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_item_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace item {

SessionMessageHandler::SessionMessageHandler() {}
SessionMessageHandler::~SessionMessageHandler() {}

bool SessionMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessageActorCompareSynchronize, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ACTOR_FACADE_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessageActorFacadeSynchronize, this, _1, _2));

  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE,
      boost::bind(&SessionMessageHandler::OnMessageItemMallConfigSynchronize, this, _1, _2));

  return true;
}

void SessionMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ACTOR_FACADE_SYNCHRONIZE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_ITEM_MALL_CONFIG_SYNCHRONIZE);
}

void SessionMessageHandler::OnMessageActorCompareSynchronize(const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageActorCompareSynchronize synchronize;
  if(global::GlobalPacket::GetPacket()->Deserialize(synchronize, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  // 获取对比玩家对象
  ItemActor *compare_actor = ItemActorManager::GetInstance()->Get(synchronize.compare_actor_);
  if(compare_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", synchronize.compare_actor_);
    return ;
  }

  // 得到 GameActor 玩家对象
  GameActor *compare_game_actor = compare_actor->GetActor();
  if(compare_game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", synchronize.compare_actor_);
    return ;
  }

  // 得到装备容器
  ItemContainer *equip_container = compare_actor->GetContainer(
      entity::ItemContainerType::EQUIP);
  if(equip_container == NULL) {
    LOG_ERROR("获取玩家(%lu) 装备容器栏失败", compare_game_actor->GetID());
    return ;
  }

  // 消息回应
  gateway::protocol::MessageEquipCompareResponse response;

  // 设置角色基本信息
  response.__set_name_(compare_game_actor->GetName());
  response.__set_level_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::LEVEL));
  response.__set_vocation_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::VOCATION));
  response.__set_gender_(compare_game_actor->GetAttribute(
        entity::ActorAoiFields::GENDER));
  response.__set_appearance_(compare_game_actor->GetAttribute(
        entity::ActorAoiFields::APPEARANCE));
  response.__set_current_hp_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::CURRENT_HP));
  response.__set_max_hp_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::MAX_HP));
  response.__set_current_mp_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::CURRENT_MP));
  response.__set_max_mp_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::MAX_MP));
  response.__set_physique_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::PHYSIQUE));
  response.__set_strength_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::STRENGTH));
  response.__set_intelligence_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::INTELLIGENCE));
  response.__set_agile_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::AGILE));
  response.__set_spirit_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::SPIRIT));
  response.__set_physics_attack_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::PHYSICS_ATTACK));
  response.__set_physics_defence_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::PHYSICS_DEFENCE));
  response.__set_magic_attack_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::MAGIC_ATTACK));
  response.__set_magic_defence_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::MAGIC_DEFENCE));
  response.__set_dodge_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::DODGE));
  response.__set_crit_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::CRIT));
  response.__set_ignore_defence_(compare_game_actor->GetAttribute(
        entity::RoleClientFields::IGNORE_DEFENCE));
  response.__set_speed_(compare_game_actor->GetAttribute(
        entity::RoleAoiFields::MOVE_SPEED));
  response.__set_current_exp_(compare_game_actor->GetAttribute(
        entity::ActorClientFields::CURRENT_EXP));
  response.__set_nimbus_(compare_game_actor->GetAttribute(
        entity::ActorClientFields::NIMBUS));
  response.__set_functionality_state_(compare_game_actor->GetFunctionalityState());

  // 设置装备信栏信息
  gateway::protocol::CompareEquip equip_info;
  for(core::uint16 pos = entity::EquipmentType::MIN;
      pos < entity::EquipmentType::MAX; ++pos) {
    Equipment *equipment = (Equipment *)equip_container->GetItem(pos);
    if(equipment) {
      equip_info.__set_template_id_(equipment->GetTemplateID());
      equip_info.__set_fields_(equipment->GetEquipClientAttributes());
      response.equipments_.push_back(equip_info);
    }
  }

  // 设置子战斗力信息
  for(core::int32 i = entity::SubFightScoreType::MIN;
      i < entity::SubFightScoreType::MAX; ++i) {
    response.sub_fight_scores_.push_back(
        compare_game_actor->GetSubFightScore((entity::SubFightScoreType::type)i));
  }

  // 多播网关，省去绕 Session 服务器
  core::uint64 actors[1] = { (core::uint64)synchronize.actor_ };
  GameServerSingleton::GetInstance().MulticastMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_COMPARE_RESPONSE, actors, 1);

  // 发送比较事件
  event::EventActorCompare event;
  event.__set_actor_(synchronize.actor_);
  event.__set_compare_actor_(compare_game_actor->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_COMPARE, compare_game_actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void SessionMessageHandler::OnMessageActorFacadeSynchronize(const char *data, size_t size) {
  if(data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  session::protocol::MessageActorFacadeSynchronize synchronize;
  if(global::GlobalPacket::GetPacket()->Deserialize(synchronize, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }

  // 获取对比玩家对象
  ItemActor *observe_actor = ItemActorManager::GetInstance()->Get(synchronize.observe_actor_);
  if(observe_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", synchronize.observe_actor_);
    return ;
  }

  // 得到 GameActor 玩家对象
  GameActor *observe_game_actor = observe_actor->GetActor();
  if(observe_game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", synchronize.observe_actor_);
    return ;
  }

  // 得到装备容器
  ItemContainer *equip_container = observe_actor->GetContainer(
      entity::ItemContainerType::EQUIP);
  if(equip_container == NULL) {
    LOG_ERROR("获取玩家(%lu) 装备容器栏失败", observe_game_actor->GetID());
    return ;
  }

  gateway::protocol::MessageActorFacadeResponse response;

  response.__set_actor_(observe_game_actor->GetStringID());
  response.__set_vocation_((entity::VocationType::type)observe_game_actor->GetAttribute(
        entity::RoleAoiFields::VOCATION));
  response.__set_gender_((entity::GenderType::type)observe_game_actor->GetAttribute(
        entity::ActorAoiFields::GENDER));
  response.__set_appearance_(observe_game_actor->GetAttribute(
        entity::ActorAoiFields::APPEARANCE));
  response.__set_sword_(observe_game_actor->GetAttribute(
        entity::ActorAoiFields::CURRENT_SWORD));
  response.__set_spirit_facade_(observe_game_actor->GetAttribute(
        entity::ActorAoiFields::CURRENT_SPIRIT_FACADE));

  entity::EquipFacadeField field;
  for(core::uint16 pos = entity::EquipmentType::MIN;
      pos < entity::EquipmentType::MAX; ++pos) {
    Equipment *equipment = (Equipment *)equip_container->GetItem(pos);
    if(equipment) {
      field.__set_location_((entity::EquipmentType::type)pos);
      field.__set_template_id_(equipment->GetTemplateID());
      field.__set_intensify_(equipment->GetAttribute(entity::EquipClientFields::INTENSIFY));
      response.equipments_.push_back(field);
    }
  }

  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(synchronize.actor_);
  if(game_actor) {
    game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_ACTOR_FACADE_RESPONSE);
  } else {
    core::uint64 actors[1] = { (core::uint64)synchronize.actor_ };
    GameServerSingleton::GetInstance().MulticastMessage(response,
        gateway::protocol::MessageType::MESSAGE_ACTOR_FACADE_RESPONSE, actors, 1);
  }
}

void SessionMessageHandler::OnMessageItemMallConfigSynchronize(const char *data, size_t size) {
  session::protocol::MessageItemMallConfigSynchronize synchronize;
  if(global::GlobalPacket::GetPacket()->Deserialize(synchronize, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return ;
  }
  for(size_t i = 0; i < synchronize.mall_config_.size(); ++i) {
    if(Configure::GetInstance()->GetMallConfigure().AddMallLine(synchronize.mall_config_[i]) == false) {
      LOG_ERROR("有重复的数据加入，错误");
    }
  }
}

}  // namespace item

}  // namespace server

}  // namespace game

