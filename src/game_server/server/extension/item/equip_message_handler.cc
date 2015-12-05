//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-13 16:01:56.
// File name: equip_message_handler.cc
//
// Description:
// Define class EquipMessageHandler.
//

#include "game_server/server/extension/item/equip_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "entity/inform_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension/item/equipment.h"
#include "game_server/server/extension/item/equipment_container.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/request/game_guild_request_types.h"
#include "game_server/server/request/game_task_request_types.h"
#include "game_server/server/request/game_welfare_request_types.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "global/common_functions.h"
#include "global/logging.h"
#include "global/types.h"
#include "global/configure/configure.h"
#include "session_server/protocol/game_actor_protocol_types.h"

namespace game {

namespace server {

namespace item {

EquipMessageHandler::EquipMessageHandler() {}
EquipMessageHandler::~EquipMessageHandler() {}

bool EquipMessageHandler::Initialize() {
  // MESSAGE_EQUIP_COMPARE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_COMPARE_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipCompare, this, _1, _2, _3));

  // MESSAGE_EQUIP_INTENSIFY_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_INTENSIFY_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipIntensify, this, _1, _2, _3));
 
  // MESSAGE_EQUIP_REFINE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipRefine, this, _1, _2, _3));

  // MESSAGE_EQUIP_UPGRADE_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_UPGRADE_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipUpgrade, this, _1, _2, _3));

  // MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipRefineConfirm, this, _1, _2, _3));

  // MESSAGE_EQUIP_INHERIT_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_INHERIT_REQUEST,
      boost::bind(&EquipMessageHandler::OnMessageEquipInherit, this, _1, _2, _3));

  return true;
}

void EquipMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_COMPARE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_INTENSIFY_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_UPGRADE_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_EQUIP_INHERIT_REQUEST);
}

static void SendMessageEquipInheritResponseFailed(ItemActor *item_actor) {
  gateway::protocol::MessageEquipInheritResponse response;
  response.__set_result_(false);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_INHERIT_RESPONSE);
}

void EquipMessageHandler::OnMessageEquipCompare(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到(%lu)", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageEquipCompareRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageEquipCompareRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_COMPARE_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str());

  // 获取对比玩家对象
  ItemActor *compare_actor = ItemActorManager::GetInstance()->Get(
      atol(message.actor_id_.c_str()));
  if(compare_actor == NULL) {
    // 玩家不在当前服务器，发送到Session上
    session::protocol::MessageActorCompareRequest request;
    request.__set_actor_(id);
    request.__set_compare_actor_(atol(message.actor_id_.c_str()));
    SessionChannel::SendMessage(message,
        session::protocol::GameMessageType::MESSAGE_ACTOR_COMPARE_REQUEST);
    return ;
  }

  // 得到 GameActor 玩家对象
  GameActor *compare_game_actor = compare_actor->GetActor();
  if(compare_game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", message.actor_id_.c_str());
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

  request::RequestGuildGetActorGuildName request;
  request.__set_actor_id_(compare_game_actor->GetID());
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD_NAME, &request,
        sizeof(request))) {
    LOG_ERROR("请求 REQUEST_GUILD_GET_ACTOR_GUILD_NAME 失败");
    return;
  }
  response.__set_guild_name_(request.guild_name_);

  // 设置角色基本信息
  response.__set_actor_id_(compare_game_actor->GetStringID());
  response.__set_name_(compare_game_actor->GetName());
  response.__set_spirit_facade_(compare_game_actor->GetAttribute(
        entity::ActorAoiFields::CURRENT_SPIRIT_FACADE));
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
  // 获取纹章信息
  request::RequestTaskMaxLayer request_layer;
  request_layer.__set_actor_id_(id);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_TASK_MAX_LAYER, &request_layer,
        sizeof(request_layer)) == -1) {
    LOG_ERROR("玩家[%lu]纹章ID没有找到", id);
    return ;
  }
  response.__set_task_layer_(request_layer.task_layer_);

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

  // 设置称号ID
  request::RequestWelfareGetTitleID request_title;
  request_title.__set_actor_id_(id);
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_WELFARE_GET_TITLE_ID,
        &request_title, sizeof(request_title)) == -1) {
    LOG_ERROR("获取称号ID失败(%lu)", id);
    return ;
  }
  response.__set_title_(request_title.title_id_);

  // 发送消息
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_COMPARE_RESPONSE);

  // 发送比较事件
  event::EventActorCompare event;
  event.__set_actor_(actor->GetID());
  event.__set_compare_actor_(compare_game_actor->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_COMPARE, compare_game_actor->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}

void EquipMessageHandler::OnMessageEquipIntensify(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageEquipIntensifyRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageEquipIntensifyRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_INTENSIFY_REQUEST from actor(%lu, %s), container_(%d), id_(%s), location_(%d), spend_freedom_item_(%d), vocation_(%d)", 
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.id_.c_str(), message.location_, message.spend_freedom_item_, message.vocation_);

  entity::VocationType::type vocation_type = entity::VocationType::MAX;
  if(message.__isset.vocation_ == true) {
    vocation_type = message.vocation_;
  }

  // 获取相应容器
  ItemContainer *container = actor->GetContainer(message.container_, vocation_type);
  if(container == NULL) {
    LOG_WARNING("获取道具容器(%d)失败", message.container_);
    return ;
  }

  // 获取道具
  Item *item = container->GetItem(message.location_);
  if(item == NULL) {
    LOG_WARNING("获取道具失败");
    return ;
  }

  // 判断道具ID是否相同
  core::uint64 item_id = atol(message.id_.c_str());
  if(item->GetID() != item_id) {
    LOG_WARNING("操作失败，道具ID错误");
    return ;
  }

  // 检测是否是装备
  if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("操作失败，该道具不是装备");
    return ;
  }

  Equipment *equipment = (Equipment *)item;

  // 检测不能过期
  if(equipment->CheckExpire() == true) {
    LOG_WARNING("过期装备，不能强化");
    return ;
  }

  const EquipmentCell *equip_cell = Configure::GetInstance()->GetEquipmentConfigure(
      equipment->GetTemplateID());
  if(equip_cell == NULL) {
    LOG_WARNING("获取装备(%u)配置失败", equipment->GetTemplateID());
    return ;
  }

  core::int32 max_intensify =
    equipment->GetAttribute(entity::EquipClientFields::MAX_INTENSIFY);
  core::int32 intensify =
    equipment->GetAttribute(entity::EquipClientFields::INTENSIFY);

  // 不能超过装备表中的最大强化等级
  if(equip_cell->max_intensify_ < (core::uint32)(intensify + 1)) {
    LOG_WARNING("已到最大强化等待，不能再进行强化");
    return ;
  }

  const IntensifyCell *cell = Configure::GetInstance()->GetIntensifyConfigure(
      equip_cell->level_, intensify + 1);
  if(cell == NULL) {
    LOG_WARNING("获取装备强化(%u,%u)配置失败", equip_cell->level_, intensify + 1);
    return ;
  }

  PacketContainer *packet = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(packet == NULL) {
    LOG_WARNING("获取背包容器失败");
    return ;
  }

  entity::BindRequireType::type bind =
    message.spend_freedom_item_ ? entity::BindRequireType::NOT_BIND : entity::BindRequireType::BOTH;

  // 判断道具是否足够
  if(packet->GetItemNumber(cell->spend_item_, bind) < (core::uint32)cell->spend_item_num_) {
    LOG_WARNING("玩家道具不足，不能进行强化");
    return ;
  }

  // 判断金币是否足够
  core::int32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
  if(gold < cell->spend_gold_) {
    LOG_WARNING("玩家金币不足，不能进行强化");
    return ;
  }

  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  core::uint32 odds = cell->min_odds_;
  if(max_intensify >= intensify + 1) {
    odds = cell->max_odds_;
  }

  // 扣除金币和道具 
  game_actor->AddResource(entity::ResourceID::GOLD, 0 - cell->spend_gold_);
  packet->RemoveItem(cell->spend_item_, cell->spend_item_num_, bind);
  if(bind == entity::BindRequireType::BIND) {
    // 消耗了绑定道具，装备变成绑定
    equipment->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
  }

  gateway::protocol::MessageEquipIntensifyResponse response;
  event::EventItemEquipStrength event;
  if(rand <= odds) {
    // 随机成功
    ++intensify;
    response.__set_result_(true);
    event.__set_result_(true);
  } else {
    // 失败，倒退
    if(intensify > 0) {
      --intensify;
    }
    event.__set_result_(false);
  }

  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    // 正穿在身上时
    equipment->StopMainEffect(equip_cell);
    equipment->SetAttribute(entity::EquipClientFields::INTENSIFY, intensify);
    equipment->StartMainEffect(equip_cell);
    equipment->UpdateBroadcast((entity::EquipmentType::type)equipment->GetLocation(),
        true);
  } else {
    equipment->SetAttribute(entity::EquipClientFields::INTENSIFY, intensify);
  }

  // 设置最大强化等级
  if(intensify > max_intensify) {
    equipment->SetAttribute(entity::EquipClientFields::MAX_INTENSIFY, intensify);
  }

  // 更新战斗力
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)container;
    equipment_container->UpdateEquipmentFightScore();
  }

  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_INTENSIFY_RESPONSE);

  // 发送事件通知
  event.__set_id_(id);
  event.__set_template_id_(equipment->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ITEM_EQUIP_STRENGTH, game_actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  if (max_intensify < intensify && intensify >= 7 && intensify <= 9) {
    // 世界公告: 装备强化到高等级(7~9)
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::EQUIPMENT_INTENSIFY_HIGH_LEVEL);
    inform.params_.push_back(game_actor->GetStringID());
    inform.params_.push_back(game_actor->GetName());
    inform.params_.push_back(global::ToString(equipment->GetTemplateID()));
    inform.params_.push_back(global::ToString(intensify));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  } else if (intensify == (core::int32)equip_cell->max_intensify_) {
    // 世界公告: 装备强化到最高等级
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::EQUIPMENT_INTENSIFY_TOP_LEVEL);
    inform.params_.push_back(game_actor->GetStringID());
    inform.params_.push_back(game_actor->GetName());
    inform.params_.push_back(global::ToString(equipment->GetTemplateID()));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }
}

void EquipMessageHandler::OnMessageEquipUpgrade(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 获取 GameActor 对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%lu) GameActor 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageEquipUpgradeRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("%s:%d (%s) 解析消息 MessageEquipUpgradeRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_UPGRADE_REQUEST from actor(%lu, %s), id_(%s), location_(%d), spend_freedom_item_(%d), type_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_.c_str(), message.location_,
      message.spend_freedom_item_, message.type_, message.vocation_);

  entity::VocationType::type vocation_type = entity::VocationType::MAX;
  if(message.__isset.vocation_ == true) {
    vocation_type = message.vocation_;
  }

  // 得到对应容器
  ItemContainer *container = actor->GetContainer(message.type_, vocation_type);
  if(container == NULL) {
    LOG_WARNING("获取玩家(%lu) 装备容器栏失败", id);
    return ;
  }

  // 得到相应位置上的道具
  Item *item = container->GetItem(message.location_);
  if(item == NULL) {
    LOG_WARNING("获取容器内的道具失败");
    return ;
  }

  // 判断道具ID是否相同
  core::uint64 item_id = atol(message.id_.c_str());
  if(item->GetID() != item_id) {
    LOG_WARNING("操作失败，道具ID错误");
    return ;
  }

  // 不是装备，不能升级
  if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("请求升级的道具不是装备");
    return ;
  }

  // 是否可以从容器中删除
  if(container->CheckRemoveItem(item) == false) {
    LOG_WARNING("升阶失败，无法从容器中删除该道具");
    return ;
  }

  Equipment *equipment = (Equipment *)item;

  // 检测不能过期
  if(equipment->CheckExpire() == true) {
    LOG_WARNING("过期装备，不能强化");
    return ;
  }

  core::int32 old_template_id = equipment->GetTemplateID();

  // 获取装备配置
  const EquipmentCell *old_cell = Configure::GetInstance()->GetEquipmentConfigure(
      equipment->GetTemplateID());
  if(old_cell == NULL) {
    LOG_WARNING("获取装备(%u)配置失败", equipment->GetTemplateID());
    return ;
  }

  if(old_cell->upgrade_id_ == 0) {
    LOG_WARNING("装备(%u) 不能进行升级", equipment->GetTemplateID());
    return ;
  }

  // 新装备配置
  const EquipmentCell *new_cell = Configure::GetInstance()->GetEquipmentConfigure(
      old_cell->upgrade_id_);
  if(new_cell == NULL) {
    LOG_WARNING("获取装备(%u)配置失败", old_cell->upgrade_id_);
    return ;
  }

  // 获取背包容器
  PacketContainer *packet = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(packet == NULL) {
    LOG_WARNING("获取背包容器失败");
    return ;
  }

  // 判断金币是否足够
  core::uint32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
  if(gold < old_cell->upgrade_spend_gold_) {
    LOG_WARNING("玩家金币不足，不能进行升阶");
    return ;
  }

  // 判断道具是否足够
  entity::BindRequireType::type bind =
    message.spend_freedom_item_ ? entity::BindRequireType::NOT_BIND : entity::BindRequireType::BOTH;
  if(packet->GetItemNumber(old_cell->upgrade_spend_item_, bind) <
      (core::uint32)old_cell->upgrade_spend_item_num_) {
    LOG_WARNING("玩家道具数量不足，不能进行升阶");
    return ;
  }

  // 成功率=(当前幸运-int(幸运上限*系数))/(幸运上限-int(幸运上限*系数))
  // 幸运值小时幸运上限*3/4时，成功率小于零，取值零
  core::int32 lucky_value =
    equipment->GetAttribute(entity::EquipClientFields::UPGRADE_LUCKY);
  core::int32 mid_value = old_cell->upgrade_max_lucky_ *
    old_cell->upgrade_lucky_coe_ / global::kPercentDenom;
  core::int32 numerator = lucky_value - mid_value;
  core::int32 denom = old_cell->upgrade_max_lucky_ - mid_value;
  core::int32 odds = 0;
  if(denom != 0) {
    odds = numerator * global::kPercentDenom / denom;
    if(odds < 0) {
      odds = 0;
    }
  }
  odds += old_cell->upgrade_odds_;

  // 扣除金币和道具
  game_actor->AddResource(entity::ResourceID::GOLD, 0 - old_cell->upgrade_spend_gold_);
  packet->RemoveItem(old_cell->upgrade_spend_item_,
      old_cell->upgrade_spend_item_num_, bind);

  // 随机数
  core::uint32 rand =
    ExtensionManager::GetInstance()->GetRandom().Random(global::kPercentDenom);

  // 失败
  if(rand >= (core::uint32)odds) {
    if((core::uint32)lucky_value < old_cell->upgrade_max_lucky_) {
      core::int32 add_lucky = rand % old_cell->upgrade_lucky_;
      lucky_value += (add_lucky <= 0 ? old_cell->upgrade_lucky_ : add_lucky);
      if((core::uint32)lucky_value > old_cell->upgrade_max_lucky_) {
        lucky_value = old_cell->upgrade_max_lucky_;
      }
      equipment->SetAttribute(entity::EquipClientFields::UPGRADE_LUCKY,
          lucky_value);
    }
    if(bind == entity::BindRequireType::BIND) {
      // 消耗了绑定道具，装备变成绑定
      equipment->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
    }
    gateway::protocol::MessageEquipUpgradeResponse response;
    response.__set_result_(false);
    game_actor->SendMessage(response,
        gateway::protocol::MessageType::MESSAGE_EQUIP_UPGRADE_RESPONSE);

    return ;
  }

  // 成功
  Equipment *equip_new = (Equipment *)ItemBuilder::GetInstance()->Create(actor,
      entity::ItemType::EQUIPMENT, old_cell->upgrade_id_);
  if(equip_new == NULL) {
    LOG_WARNING("玩家(%lu) 升阶装备(%u)时，创建装备失败",
        id, equipment->GetID());
    return ;
  }

  // 从容器中删除道具
  if(container->RemoveItem(item) == false) {
    ItemBuilder::GetInstance()->Destory(equip_new);
    LOG_ERROR("玩家(%lu) 升阶装备(%u)时，删除道具失败");
    return ;
  }

  // 检测可否增加道具
  if(container->CheckAddItem(equip_new, message.location_) == false) {
    ItemBuilder::GetInstance()->Destory(equip_new);
    container->AddItem(item, message.location_);
    LOG_ERROR("玩家(%lu) 升阶装备(%u)时，向容器增加道具失败");
    return ;
  }

  if(bind == 1 || equipment->GetAttribute(entity::ItemClientFields::BIND_TYPE) == 1) {
    // 消耗了绑定道具，装备变成绑定
    equip_new->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1);
  }

  // 老装备的强化等级
  core::int32 old_equip_level =
    equipment->GetAttribute(entity::EquipClientFields::INTENSIFY);

  // 幸运值清0
  equip_new->SetAttribute(entity::EquipClientFields::UPGRADE_LUCKY, 0);
  // 保留强化等级
  equip_new->SetAttribute(entity::EquipClientFields::INTENSIFY, old_equip_level);
  // 历史最高强化等级设置为当前强化等级
  equip_new->SetAttribute(entity::EquipClientFields::MAX_INTENSIFY, old_equip_level);

  // 随机属性升级
  const RandomAttributeCell *random_cell = Configure::GetInstance()->GetRandomAttribute(
      new_cell->random_attr_id_);
  if(random_cell != NULL) {
    const LoadRandomAttribute &random_configure =
      Configure::GetInstance()->GetLoadRandomeAttribute();
    const LoadRandomAttribute::AttributeVector *attr_vector =
      random_configure.GetRandomVector(new_cell->random_attr_id_);
    if(attr_vector != NULL) {
      // 升阶需要保留之前的随机属性个数和种类，但数值需要使用新的随机属性ID配置
      LoadRandomAttribute::AttributeVector::const_iterator iterator =
        attr_vector->begin();
      for(; iterator != attr_vector->end(); ++iterator) {
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_1,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_1,
              iterator->value_);
        }
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_2)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_2,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_2));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_2,
              iterator->value_);
        }
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_3)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_3,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_3));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_3,
              iterator->value_);
        }
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_4)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_4,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_4));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_4,
              iterator->value_);
        }
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_5)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_5,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_5));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_5,
              iterator->value_);
        }
        if(iterator->type_ == equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_6)) {
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_6,
              equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_6));
          equip_new->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_6,
              iterator->value_);
        }
      }
    } else {
      LOG_ERROR("玩家(%lu) 升阶装备(%u)时，没找到对应配置中的随机属性(%u)",
          id, equip_new->GetTemplateID(), new_cell->random_attr_id_);
    }
  } else {
    LOG_ERROR("玩家(%lu) 升阶装备(%u)时，没找到对应配置中的随机属性(%u)",
        id, equip_new->GetTemplateID(), new_cell->random_attr_id_);
  }

  // 增加到对应容器
  if(container->AddItem(equip_new, message.location_) == false) {
    ItemBuilder::GetInstance()->Destory(equip_new);
    container->AddItem(item, message.location_, true);
    LOG_ERROR("玩家(%lu) 升阶装备(%u)时，向容器增加道具失败");
    return ;
  }

  ItemBuilder::GetInstance()->Destory(item);

  // 更新战斗力
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)container;
    equipment_container->UpdateEquipmentFightScore();
  }

  gateway::protocol::MessageEquipUpgradeResponse response;
  response.__set_result_(true);
  response.__set_id_(equip_new->GetStringID());
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_UPGRADE_RESPONSE);

  // 世界公告: 装备进阶成功
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::EQUIPMENT_UPGRADE);
  inform.params_.push_back(game_actor->GetStringID());
  inform.params_.push_back(game_actor->GetName());
  inform.params_.push_back(global::ToString(old_template_id));
  inform.params_.push_back(global::ToString(old_cell->upgrade_id_));
  GameServerSingleton::GetInstance().BroadcastMessage(inform,
      gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
}

void EquipMessageHandler::OnMessageEquipRefine(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }
  
  //获取道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu)对象失败", id);
    return ;
  }
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取gameActor(%lu)失败", id);
    return ;
  }

  // 解析协议
  gateway::protocol::MessageEquipRefineRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageEquipRefineRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_REFINE_REQUEST from actor(%lu, %s), container_(%d), location_(%d), lock1_(%d), lock2_(%d), lock3_(%d), lock4_(%d), lock5_(%d), lock6_(%d), spend_freedom_item_(%d),vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.location_,
      message.lock1_, message.lock2_, message.lock3_, message.lock4_, message.lock5_, message.lock6_,
      message.spend_freedom_item_, message.vocation_);

  entity::VocationType::type vocation_type = entity::VocationType::MAX;
  if(message.__isset.vocation_ == true) {
    vocation_type = message.vocation_;
  }

  // 获取相应容器
  ItemContainer *container = actor->GetContainer(message.container_, vocation_type);
  if(container == NULL) {
    LOG_WARNING("获取容器道具(%d)失败", message.container_);
    return ;
  }

  // 获取道具
  Item *item = container->GetItem(message.location_);
  if(item == NULL) {
    LOG_WARNING("获取道具失败");
    return ;
  }

  //检测是否是装备
  if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("操作失败，该道具不是装备");
    return ;
  }

  Equipment *equipment = (Equipment *)item;

  //检测不能过期
  if(equipment->CheckExpire() == true) {
    LOG_WARNING("过期装备 不能洗练");
    return ;
  }
  
  // 获取装备配置
  const EquipmentCell *equipcell = Configure::GetInstance()->GetEquipmentConfigure(
       equipment->GetTemplateID());
  if(equipcell == NULL) {
    LOG_WARNING("获取装备(%u)配置失败", equipment->GetTemplateID());
    return ;
  }

  // 获取配置
  const EquipRefineSpendCell *equip_refine_cell =
    Configure::GetInstance()->GetRefineConfigure(equipcell->level_, equipcell->quality_);
  if(equip_refine_cell == NULL) {
    LOG_WARNING("装备(%u)洗练消耗 配置没有找到", equipment->GetTemplateID());
    return ;
  }

  // 获取协议信息
  core::int32 lock_num = 0;
  core::int32 spend_diamond = 0;
  message.lock1_ == true ? lock_num++ : 0;
  message.lock2_ == true ? lock_num++ : 0;
  message.lock3_ == true ? lock_num++ : 0;
  message.lock4_ == true ? lock_num++ : 0;
  message.lock5_ == true ? lock_num++ : 0;
  message.lock6_ == true ? lock_num++ : 0;
  const EquipRefineSpendLockCell *equip_lock_cell =
    Configure::GetInstance()->GetRefineLockConfigure(lock_num);
  if(equip_lock_cell != NULL) {
    spend_diamond = equip_lock_cell->spend_diamond_;
  }
 
  // 获取背包容器
  PacketContainer *bag = (PacketContainer *)actor->GetContainer(
     entity::ItemContainerType::PACKET);
  if(bag == NULL) {
     LOG_WARNING("获取背包容器失败");
     return ;
  }

  entity::BindRequireType::type bind =
      message.spend_freedom_item_ ? entity::BindRequireType::NOT_BIND : entity::BindRequireType::BOTH;
 
  // 检测消耗
  if(bag->GetItemNumber((core::uint32)equip_refine_cell->spend_item_, bind) < 
        (core::uint32)equip_refine_cell->spend_item_num_) {
     LOG_WARNING("洗练消耗的道具不足");
     return ;
  }
  core::int32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
  if(gold < equip_refine_cell->spend_gold_) {
    LOG_WARNING("洗练消耗的金币不足");
    return ;
  }
  core::int32 diamond = game_actor->GetAttribute(
      entity::ActorClientFields::FREEDOM_DOLLARS);
  if(diamond < spend_diamond) {
    LOG_WARNING("洗练消耗的钻石不足");
    return ;
  }

  // 处理消耗
  bag->RemoveItem((core::uint32)equip_refine_cell->spend_item_,
      (core::uint32)equip_refine_cell->spend_item_num_, bind);
  game_actor->AddResource(entity::ResourceID::GOLD, 0 - equip_refine_cell->spend_gold_);
  if(bind == entity::BindRequireType::BIND) {
    equipment->SetAttribute(entity::ItemClientFields::BIND_TYPE, 1); 
  }
  // 处理随机属性
  for(core::uint32 pos = 0; pos < equipcell->random_attr_number_ &&
      pos < (core::uint32)entity::g_constants_constants.kMaxRandomAttriNum; ++pos) {
    bool is_lock = false; //处理协议
    switch(pos) {
      case 0:
        is_lock = message.lock1_;
        break;
      case 1:
        is_lock = message.lock2_;
        break;
      case 2:
        is_lock = message.lock3_;
        break;
      case 3:
        is_lock = message.lock4_;
        break;
      case 4:
        is_lock = message.lock5_;
        break;
      case 5:
        is_lock = message.lock6_;
        break;
      default:
        is_lock = true;
        break;
    }
    if(is_lock) {
      actor->GetEquipRefineData()->rand_attr_type_[pos] = equipment->GetAttribute((
            entity::EquipClientFields::type)(entity::EquipClientFields::RANDOM_ATTR_1 + pos * 2));
      actor->GetEquipRefineData()->rand_attr_value_[pos] = equipment->GetAttribute((
            entity::EquipClientFields::type)(entity::EquipClientFields::RANDOM_VALUE_1 + pos * 2));
    } else {
      const RandomAttributeCell *random_cell = Configure::GetInstance()->GetRandomAttribute(
          equipcell->random_attr_id_);
      if(random_cell == NULL) {
        global::LogError("%s:%d (%s) 道具ID(%u) 随机属性ID(%u) 随机失败",
            __FILE__, __LINE__, __FUNCTION__, equipment->GetTemplateID(), equipcell->random_attr_id_);
        continue;
      }
      actor->GetEquipRefineData()->rand_attr_type_[pos] = random_cell->type_;
      actor->GetEquipRefineData()->rand_attr_value_[pos] = random_cell->value_;
    }
  }
  
  // 通知客户端,并且保存
  gateway::protocol::MessageEquipRefineResponse response;
  response.__set_id_(equipment->GetStringID());
  response.__set_ret_ok_(true);
  for(core::uint32 pos = 0; pos < equipcell->random_attr_number_ &&
      pos < (core::uint32)entity::g_constants_constants.kMaxRandomAttriNum; ++pos) {
    if(pos == 0) {
      response.__set_new_random_type1_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value1_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    } else if(pos == 1) {
      response.__set_new_random_type2_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value2_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    } else if(pos == 2) {
      response.__set_new_random_type3_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value3_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    } else if(pos == 3) {
      response.__set_new_random_type4_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value4_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    } else if(pos == 4) {
      response.__set_new_random_type5_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value5_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    } else if(pos == 5) {
      response.__set_new_random_type6_(actor->GetEquipRefineData()->rand_attr_type_[pos]);
      response.__set_new_random_value6_(actor->GetEquipRefineData()->rand_attr_value_[pos]);
    }
  }
  actor->GetEquipRefineData()->equip_id_ = equipment->GetID();
  actor->GetEquipRefineData()->iseffect_ = true;
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_RESPONSE);
  
  // 发送事件通知
  event::EventItemEquipRefine event;
  event.__set_id_(id);
  event.__set_template_id_(equipment->GetTemplateID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ITEM_EQUIP_REFINE, game_actor->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
}


void EquipMessageHandler::OnMessageEquipRefineConfirm(core::uint64 id, const char *data,
    size_t size) {
  // 获取道具玩家对象
  ItemActor *actor = ItemActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  // 获取 GameActor 对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%lu) GameActor 失败", id);
    return ;
  }

  if(actor->GetEquipRefineData()->iseffect_ == false) {
    LOG_WARNING("随机属性无效");
    return ;
  }

  gateway::protocol::MessageEquipRefineConfirmRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("%s:%d (%s) 解析消息 MessageEquipRefineConfirmRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_REFINE_CONFIRM_REQUEST from actor(%lu, %s), container_(%d), location_(%d), ok_(%d), vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.container_, message.location_,
      message.ok_, message.vocation_);

  // 客户端确认需要新的随机属性
  if(message.ok_ == false) {
    actor->GetEquipRefineData()->iseffect_ = false;
    return ;
  }

  entity::VocationType::type vocation_type = entity::VocationType::MAX;
  if(message.__isset.vocation_ == true) {
    vocation_type = message.vocation_;
  }

  // 获取相应容器
  ItemContainer *container = actor->GetContainer(message.container_, vocation_type);
  if(container == NULL) {
    LOG_WARNING("获取容器道具(%d)失败", message.container_);
    return ;
  }

  // 获取道具
  Item *item = container->GetItem(message.location_);
  if(item == NULL) {
    LOG_WARNING("获取道具失败");
    return ;
  }

  //检测是否是装备
  if(item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("操作失败， 该道具不是装备");
    return ;
  }

  Equipment *equipment = (Equipment *)item;
  
  // 检测装备是否是处于正在洗练的状态
  if(actor->GetEquipRefineData()->equip_id_ != equipment->GetID()) {
    LOG_WARNING("装备处于不在洗练的状态 %lu != %lu",
        actor->GetEquipRefineData()->equip_id_, equipment->GetID());
    return ;
  }

 // 获取装备配置
  const EquipmentCell *equipcell = Configure::GetInstance()->GetEquipmentConfigure(
       equipment->GetTemplateID());
  if(equipcell == NULL) {
    LOG_WARNING("获取装备(%u)配置失败", equipment->GetTemplateID());
    return ;
  }
  
  // 如果在装备栏，关闭效果
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    if(equipment->CheckExpire() == false) {
      equipment->StopRandEffect(equipcell);
    }
  }
  
  // 处理随机属性
  for(core::uint32 pos = 0; pos < equipcell->random_attr_number_ &&
      pos < (core::uint32)entity::g_constants_constants.kMaxRandomAttriNum; ++pos) {
    equipment->SetAttribute((entity::EquipClientFields::type)(entity::EquipClientFields::RANDOM_ATTR_1 + pos * 2),
        actor->GetEquipRefineData()->rand_attr_type_[pos]);
    equipment->SetAttribute((entity::EquipClientFields::type)(entity::EquipClientFields::RANDOM_VALUE_1 + pos * 2),
        actor->GetEquipRefineData()->rand_attr_value_[pos]);
  }
  actor->GetEquipRefineData()->iseffect_ = false;
  actor->GetEquipRefineData()->equip_id_ = 0;

  // 开启效果
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    //检测不能过期
    if(equipment->CheckExpire() == false) {
      equipment->StartRandEffect(equipcell);
    }
  }

  // 更新战斗力
  if(container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)container;
    equipment_container->UpdateEquipmentFightScore();
  }

  // 通知客户端
  gateway::protocol::MessageEquipRefineConfirmResponse response;
  response.__set_ok_(true);
  response.__set_id_(equipment->GetStringID());
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_REFINE_CONFIRM_RESPONSE);

  if (equipcell->quality_ >= 4 &&
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1) ==
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_2) &&
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1) ==
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_3) &&
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1) ==
      equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_4) &&
      (equipcell->random_attr_number_ < 5 || equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1) ==
       equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_5)) &&
      (equipcell->random_attr_number_ < 6 || equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1) ==
       equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_6))) {
    // 世界公告: 装备洗练
    gateway::protocol::MessageInformNotify inform;
    inform.__set_id_(entity::InformType::EQUIPMENT_REFINE);
    inform.params_.push_back(game_actor->GetStringID());
    inform.params_.push_back(game_actor->GetName());
    inform.params_.push_back(global::ToString(equipment->GetTemplateID()));
    inform.params_.push_back(global::ToString(
          equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1)));
    GameServerSingleton::GetInstance().BroadcastMessage(inform,
        gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
  }
}

void EquipMessageHandler::OnMessageEquipInherit(core::uint64 id, const char *data,
    size_t size)
{
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 得到道具玩家对象
  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(id);
  if(item_actor == NULL) {
    LOG_ERROR("获取玩家(%lu) 失败", id);
    return ;
  }

  GameActor *game_actor = item_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageEquipInheritRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageEquipInheritRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_EQUIP_INHERIT_REQUEST from actor(%lu, %s), src_location_(%d), src_type_(%d), src_vocation_(%d), dest_location_(%d), dest_type_(%d), dest_vocation_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.src_location_, message.src_type_,
      message.src_vocation_, message.dest_location_, message.dest_type_, message.dest_vocation_);

  entity::VocationType::type src_vocation_type = entity::VocationType::MAX;
  entity::VocationType::type dest_vocation_type = entity::VocationType::MAX;
  if(message.__isset.src_vocation_ == true) {
    src_vocation_type = message.src_vocation_;
  }
  if(message.__isset.dest_vocation_ == true) {
    dest_vocation_type = message.dest_vocation_;
  }

  // 得到源装备
  ItemContainer *src_container =
      item_actor->GetContainer(message.src_type_, src_vocation_type);
  if(src_container == NULL) {
    LOG_WARNING("获取玩家(%lu) 源装备容器栏失败", id);
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  Item *src_item = src_container->GetItem(message.src_location_);
  if(src_item == NULL) {
    LOG_WARNING("获取源道具失败");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  if(src_item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("源道具不是装备");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  Equipment *src_equipment = (Equipment *)src_item;
  if (src_equipment->CheckExpire() == true) {
    LOG_WARNING("源装备已过期");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 检查是否可以删除源装备
  if (src_container->CheckRemoveItem(src_item) == false) {
    LOG_WARNING("无法删除源道具");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 得到目标装备
  ItemContainer *dest_container =
      item_actor->GetContainer(message.dest_type_, dest_vocation_type);
  if(dest_container == NULL) {
    LOG_WARNING("获取玩家(%lu) 目标装备容器栏失败", id);
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  Item *dest_item = dest_container->GetItem(message.dest_location_);
  if(dest_item == NULL) {
    LOG_WARNING("获取目标装备失败");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  if(dest_item->GetItemType() != entity::ItemType::EQUIPMENT) {
    LOG_WARNING("目标道具不是装备");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  Equipment *dest_equipment = (Equipment *)dest_item;
  if (dest_equipment->CheckExpire() == true) {
    LOG_WARNING("目标装备已过期");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 得到源装备配置
  const ItemCell *src_item_cell =
      Configure::GetInstance()->GetItem(src_equipment->GetTemplateID());
  if(src_item_cell == NULL) {
    LOG_ERROR("获取道具(%u)配置失败", src_equipment->GetTemplateID());
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  const EquipmentCell *src_equipment_cell =
      Configure::GetInstance()->GetEquipmentConfigure(src_equipment->GetTemplateID());
  if(src_equipment_cell == NULL) {
    LOG_ERROR("获取装备(%u)配置失败", src_equipment->GetTemplateID());
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 得到目标装备配置
  const ItemCell *dest_item_cell =
      Configure::GetInstance()->GetItem(dest_equipment->GetTemplateID());
  if(dest_item_cell == NULL) {
    LOG_ERROR("获取道具(%u)配置失败", dest_equipment->GetTemplateID());
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  const EquipmentCell *dest_equipment_cell =
      Configure::GetInstance()->GetEquipmentConfigure(dest_equipment->GetTemplateID());
  if(dest_equipment_cell == NULL) {
    LOG_ERROR("获取装备(%u)配置失败", dest_equipment->GetTemplateID());
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  const RandomAttributeCell *random_cell = Configure::GetInstance()->GetRandomAttribute(
      dest_equipment_cell->random_attr_id_);
  if(random_cell == NULL) {
    LOG_ERROR("获取装备随机属性配置(%u)失败", dest_equipment_cell->random_attr_id_);
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 检查源装备和目标装备的等级
  if(src_item_cell->level_ < (core::uint32)MISC_CONF()->equip_inherit_min_level_) {
    LOG_WARNING("源装备等级太低");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  if(dest_item_cell->level_ < (core::uint32)MISC_CONF()->equip_inherit_min_level_) {
    LOG_WARNING("目标装备等级太低");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 检查源装备和目标装备的品质
  if(src_item_cell->quality_ < MISC_CONF()->equip_inherit_min_src_quality_)
  {
    LOG_WARNING("源装备品质太低");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  if(dest_item_cell->quality_ < MISC_CONF()->equip_inherit_min_dest_quality_)
  {
    LOG_WARNING("目标装备品质太低");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 检查源装备和目标装备是否是同一职业同一部件
  if(src_equipment_cell->sub_type_ != dest_equipment_cell->sub_type_ ||
     src_equipment_cell->vocation_ != dest_equipment_cell->vocation_)
  {
    LOG_WARNING("源装备和目标装备不是同一职业同一部件");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 获取背包容器
  PacketContainer *packet = (PacketContainer *)item_actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(packet == NULL) {
    LOG_ERROR("获取背包容器失败");
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }
  // 判断道具是否足够
  if(packet->GetItemNumber(MISC_CONF()->equip_inherit_spend_item_id_,
         entity::BindRequireType::BOTH) <
     (core::uint32)MISC_CONF()->equip_inherit_spend_item_count_) {
    LOG_WARNING("玩家道具(%u)数量不足", MISC_CONF()->equip_inherit_spend_item_id_);
    SendMessageEquipInheritResponseFailed(item_actor);
    return ;
  }

  // 保存继承的属性
  core::int32 intensity = src_equipment->GetAttribute(entity::EquipClientFields::INTENSIFY);
  core::int32 random_attr_1 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_1);
  core::int32 random_attr_2 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_2);
  core::int32 random_attr_3 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_3);
  core::int32 random_attr_4 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_4);
  core::int32 random_attr_5 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_5);
  core::int32 random_attr_6 = src_equipment->GetAttribute(entity::EquipClientFields::RANDOM_ATTR_6);

  // 随机属性
  const LoadRandomAttribute::AttributeVector *random_attr_vector =
    Configure::GetInstance()->GetLoadRandomeAttribute().GetRandomVector(
        dest_equipment_cell->random_attr_id_);
  if(random_attr_vector == NULL) {
    LOG_ERROR("取随机属性出错");
    SendMessageEquipInheritResponseFailed(item_actor);
    return;
  }

  // 扣除消耗
  if (src_container->RemoveItem(src_equipment) == false) {
    LOG_ERROR("删除源装备失败");
    SendMessageEquipInheritResponseFailed(item_actor);
    return;
  }
  entity::BindRequireType::type bind = entity::BindRequireType::BOTH;
  packet->RemoveItem(MISC_CONF()->equip_inherit_spend_item_id_,
      MISC_CONF()->equip_inherit_spend_item_count_, bind);

  // 继承属性
  dest_equipment->SetAttribute(entity::EquipClientFields::INTENSIFY, intensity);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_1, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_2, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_3, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_4, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_5, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_6, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_1, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_2, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_3, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_4, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_5, 0);
  dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_6, 0);

  for(LoadRandomAttribute::AttributeVector::const_iterator iter = random_attr_vector->begin();
      iter != random_attr_vector->end(); ++iter) {
    if(iter->type_ == random_attr_1) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_1, random_attr_1);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_1, iter->value_);
    }
    if(iter->type_ == random_attr_2) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_2, random_attr_2);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_2, iter->value_);
    }
    if(iter->type_ == random_attr_3) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_3, random_attr_3);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_3, iter->value_);
    }
    if(iter->type_ == random_attr_4) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_4, random_attr_4);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_4, iter->value_);
    }
    if(iter->type_ == random_attr_5) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_5, random_attr_5);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_5, iter->value_);
    }
    if(iter->type_ == random_attr_6) {
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_ATTR_6, random_attr_6);
      dest_equipment->SetAttribute(entity::EquipClientFields::RANDOM_VALUE_6, iter->value_);
    }
  }

  // 更新战斗力
  if(src_container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)src_container;
    equipment_container->UpdateEquipmentFightScore();
  }
  if(dest_container->GetType() == entity::ItemContainerType::EQUIP) {
    EquipmentContainer *equipment_container = (EquipmentContainer *)dest_container;
    equipment_container->UpdateEquipmentFightScore();
  }

  // 发送成功回复
  gateway::protocol::MessageEquipInheritResponse response;
  response.__set_result_(true);
  item_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_EQUIP_INHERIT_RESPONSE);
}

}  // namespace item

}  // namespace server

}  // namespace game

