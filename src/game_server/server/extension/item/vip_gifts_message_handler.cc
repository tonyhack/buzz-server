//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 21:23:44.
// File name: vip_gifts_message_handler.cc
//
// Description:
//

#include "game_server/server/extension/item/vip_gifts_message_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_builder.h"
#include "game_server/server/extension/item/item_configure.h"
#include "game_server/server/extension/item/item.h"
#include "game_server/server/extension/item/configure.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

VipGiftsMessageHandler::VipGiftsMessageHandler() {}
VipGiftsMessageHandler::~VipGiftsMessageHandler() {}

bool VipGiftsMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_PICK_REQUEST,
      boost::bind(&VipGiftsMessageHandler::OnRequest, this, _1, _2, _3));
  return true;
}

void VipGiftsMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
     gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_PICK_REQUEST);
}

void VipGiftsMessageHandler::OnRequest(core::uint64 id, 
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

  // 解析消息
  gateway::protocol::MessageVipGiftsPickRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息 MessageMallBuyRequest 失败");
    return ;
  }

  // 得到 GameActor 玩家对象
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("获取玩家(%s) GameActor 对象失败", id);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_VIP_GIFTS_PICK_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str());

  if(game_actor->GetAttribute(entity::ActorAoiFields::VIP) == 
      (core::int32)entity::VipType::INVALID) {
    LOG_ERROR("玩家 VIP 过期");
    return ;
  }
  if(actor->GetVipGiftsPickNum() >= 1) {
    LOG_ERROR("VIP 礼包领取过");
    return ;
  }

  // 得到配置
  const LoadVipGiftsConfigure::Awards *awards = Configure::GetInstance()->GetAwards(
      (entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP));

  // 获取背包
  ItemContainer *con = actor->GetContainer(entity::ItemContainerType::PACKET);
  if(con == NULL) {
    LOG_ERROR("没有找到玩家(%lu)背包", id);
    return ;
  }
  PacketContainer *packet = (PacketContainer *)con;
  for(core::int32 i = 0; i < 6; ++i) {
    const VipGiftAward *award = &(*awards)[i];
    if(award->type_ == (core::int32)entity::ResourceType::MIN) {
      continue ;
    }
    if(award->type_ == entity::ResourceType::RESOURCE) {
      game_actor->AddResource((entity::ResourceID::type)award->id_,
          award->number_);
    } else if(award->type_ == (core::int32)entity::ResourceType::ITEM){
      if(packet->AddItems(award->id_, award->number_, award->bind_, 0) != true) {
        LOG_ERROR("添加道具(%d)失败", award->id_);
      }
    } else {
      LOG_ERROR("资源类型(%d)错误", award->type_);
    }
  }

  actor->SetVipGiftsPickNum(actor->GetVipGiftsPickNum() + 1);

  // 消息回应
  gateway::protocol::MessageVipGiftsPickResponse response;
  response.__set_result_(true);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_PICK_RESPONSE);

}

} // namespace item

} // namespace server

} // namespace game

