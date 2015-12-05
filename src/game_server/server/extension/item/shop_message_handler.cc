//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-06 00:16:49.
// File name: shop_message_handler.cc
//
// Description:
//

#include "game_server/server/extension/item/shop_message_handler.h"

#include <boost/bind.hpp>
#include "entity/item_types.h"
#include "gateway_server/protocol/gateway_shop_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "game_server/server/extension/item/configure.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

ShopMessageHandler::ShopMessageHandler() {}
ShopMessageHandler::~ShopMessageHandler() {}

bool ShopMessageHandler::Initialize() {
  // MESSAGE_SHOP_BUY_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_BUY_REQUEST,
      boost::bind(&ShopMessageHandler::OnMessageBuyRequest, this, _1, _2, _3));

  // MESSAGE_SHOP_SELL_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_SELL_REQUEST,
      boost::bind(&ShopMessageHandler::OnMessageSellRequest, this, _1, _2, _3));
 
  // MESSAGE_SHOP_BUYBACK_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_BUYBACK_REQUEST,
      boost::bind(&ShopMessageHandler::OnMessageBuyBackRequest, this, _1, _2, _3));
  return true;
}
 
void ShopMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_BUY_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_SELL_REQUEST);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SHOP_BUYBACK_REQUEST);
}

void ShopMessageHandler::OnMessageBuyRequest(core::uint64 id, 
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

  // 得到 GameActor 玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageShopBuyRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageShopBuyRequest 失败");
    return ;
  } 

  LOG_INFO("Recevie MESSAGE_SHOP_BUY_REQUEST from actor(%lu, %s), num_(%d), template_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.num_, message.template_id_);

  // 验证
  if(Configure::GetInstance()->CheckShopTemplateID(message.template_id_) == false) {
    LOG_ERROR("商店无此道具可出售(%d)", message.template_id_);
    return ;
  }

  // 处理消息
  const ItemCell* cell = Configure::GetInstance()->GetItem(message.template_id_);
  if(cell == NULL) {
    LOG_ERROR("道具配置没有找到 id(%d)", message.template_id_);
    return ;
  }

  core::int32 gold = game_actor->GetAttribute(entity::ActorClientFields::GOLD);
  if(gold < (core::int32)cell->buy_coin_*message.num_) {
    LOG_ERROR("玩家金币不足, 实际金币(%d), 需要金币(%d)", gold, cell->buy_coin_);
    return ;
  }

  // 获取背包容器
  PacketContainer *bag = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(bag == NULL) {
    LOG_WARNING("获取背包容器失败");
    return ;
  }

  // 增加道具到背包
  if(message.num_ <= 0) {
    LOG_ERROR("玩家输入的参数不正确，数目为(%d)", message.num_);
    return ;
  }
  if(bag->CheckAddItem(message.template_id_, message.num_, true) == true) {
    if(bag->AddItems(message.template_id_, message.num_, true, 0) == false) {
      LOG_ERROR("添加道具失败");
      return ;
    }
  } else {
    LOG_ERROR("背包已满，无法添加");
    return ;
  }
  game_actor->AddResource(entity::ResourceID::GOLD,
      -(core::int32)(cell->buy_coin_*message.num_));

  // 消息回应
  gateway::protocol::MessageShopBuyResponse response;
  response.__set_template_id_(message.template_id_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SHOP_BUY_RESPONSE);
}

void ShopMessageHandler::OnMessageSellRequest(core::uint64 id, 
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

  // 得到 GameActor 玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageShopSellRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageShopSellRequest  失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_SHOP_SELL_REQUEST from actor(%lu, %s), location_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.location_);

  // 获取背包容器
  PacketContainer *bag = (PacketContainer *)actor->GetContainer(
      entity::ItemContainerType::PACKET);
  if(bag == NULL) {
    LOG_WARNING("获取背包容器失败");
    return ;
  }

  // 获取道具
  Item * item = bag->GetItem(message.location_);
  if(item == NULL) {
    LOG_ERROR("道具位置不对,没有找到道具 %d", message.location_);
    return ;
  }

  // 获取道具配置
  const ItemCell* cell = Configure::GetInstance()->GetItem(item->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("道具配置没有找到 id(%d)", item->GetTemplateID());
    return ;
  }

  // 获取玩家的商店回购列表
  ShopBuybackContainer *buyback = (ShopBuybackContainer *)actor->GetContainer(
      entity::ItemContainerType::SHOP);
  if(buyback == NULL) {
    LOG_ERROR("玩家(%lu)的回购列表没有找到", id);
    return ;
  }

  if(bag->GetEmptyPosition() == core::kuint16max) {
    LOG_ERROR("背包没有空位，不能回购");
    return ;
  }
  // 将道具移动出背包
  if(bag->RemoveItem(item , false) == false) {
    LOG_ERROR("玩家(%lu)删除道具失败", id);
    return ;
  }

  core::int32 gold = cell->sell_coin_ * item->GetAttribute(
      entity::ItemClientFields::STACK_COUNT);
  LOG_INFO("玩家出售(%d)个道具(%d), 获得(%d)金币", item->GetAttribute(
        entity::ItemClientFields::STACK_COUNT),item->GetTemplateID(), gold);
  game_actor->AddResource(entity::ResourceID::GOLD, gold);

  if(buyback->AddItem(item) == false) {
    LOG_ERROR("玩家(%lu)道具返回回购列表出错", id);
    return ;
  }

  // 消息回应
  gateway::protocol::MessageShopSellResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SHOP_SELL_RESPONSE);

}

void ShopMessageHandler::OnMessageBuyBackRequest(core::uint64 id, 
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

  // 得到 GameActor 玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageShopBuybackRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageShopBuybackRequest 失败");
    return ;
  }

  LOG_INFO("Recevie MESSAGE_SHOP_BUYBACK_REQUEST from actor(%lu, %s), location_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.location_);

  // 得到商店回购容器
  ItemContainer* container =  actor->GetContainer(entity::ItemContainerType::SHOP);
  if(container == NULL) {
    LOG_ERROR("商店回购列表容器未找到 %lu", id);
    return ;
  }

  ShopBuybackContainer *buyback = (ShopBuybackContainer*)container;
  if(buyback == NULL) {
    LOG_ERROR("商店回购列表容器未找到 %lu", id);
    return ; 
  }

  Item *item = buyback->GetItem(message.location_);
  if(item == NULL) {
    LOG_ERROR("道具未找到%lu , 位置%d", id, message.location_);
    return ;
  }

  //获取道具配置
  const ItemCell* cell = Configure::GetInstance()->GetItem(item->GetTemplateID());
  if(cell == NULL) {
    LOG_ERROR("道具配置没有找到 id(%d)", item->GetTemplateID());
    return ;
  }

  core::int32 gold = cell->sell_coin_ * item->GetAttribute(entity::ItemClientFields::STACK_COUNT);
  if(gold > game_actor->GetAttribute(entity::ActorClientFields::GOLD)) {
    LOG_ERROR("玩家(%lu)金币不足 当前(%d)需要(%d)", id, 
        game_actor->GetAttribute(entity::ActorClientFields::GOLD), gold);
    return ;
  }

  // 得到背包容器
  ItemContainer* con =  actor->GetContainer(entity::ItemContainerType::PACKET);
  if(container == NULL) {
    LOG_ERROR("背包容器未找到 %lu", id);
    return ;
  }
  PacketContainer * pack = (PacketContainer*)con;
  if(pack == NULL) {
    LOG_ERROR("背包容器未找到 %lu", id);
    return ;
  }
  core::uint16 index = pack->GetEmptyPosition();
  if(index == core::kuint16max) {
    LOG_ERROR("背包已经满了 玩家(%lu)", id);
    return ;
  }

  if(buyback->CheckRemoveItem(item) == false) {
    LOG_ERROR("检查删除道具失败,位置(%d)", message.location_);
    return ;
  }
  if(buyback->RemoveItem(message.location_) == NULL) {
    LOG_ERROR("商品回购列表中删除道具失败%lu", id);
    pack->RemoveItem(item, false);
    return ;
  }
  if(pack->AddItem(item, index, false) == false) {
    LOG_ERROR("添加道具到背包失败 %lu", id);
    return ;
  }
  game_actor->AddResource(entity::ResourceID::GOLD, 0 - gold);
  // 消息回应
  gateway::protocol::MessageShopBuybackResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_SHOP_BUYBACK_RESPONSE);

}

}  // namespace item

}  // namespace server

}  // namespace game
