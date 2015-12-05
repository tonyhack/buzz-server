//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 20:40:04.
// File name: change_vip_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/item/change_vip_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/functionality_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "gateway_server/protocol/gateway_item_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace item {

ChangeVipEventHandler::ChangeVipEventHandler() {}
ChangeVipEventHandler::~ChangeVipEventHandler() {}

bool ChangeVipEventHandler::Initialize() {
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_VIP_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ChangeVipEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ChangeVipEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ChangeVipEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventActorVipChange) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorVipChange *event = (event::EventActorVipChange *)message;

  ItemActor *item_actor = ItemActorManager::GetInstance()->Get(event->actor_);
  if(item_actor == NULL) {
    LOG_ERROR("获取 ItemActor(%lu) 失败", event->actor_);
    return ;
  }

  GameActor *actor = item_actor->GetActor();
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->actor_);
    return ;
  }

  // vip 礼包
  item_actor->SetVipGiftsPickNum(0);
  gateway::protocol::MessageVipGiftsSynchronize vip_gifts_msg;
  if(event->new_type_ != entity::VipType::INVALID) {
    vip_gifts_msg.__set_status_(true);
  } else {
    vip_gifts_msg.__set_status_(false);
  }
  actor->SendMessage(vip_gifts_msg,
      gateway::protocol::MessageType::MESSAGE_VIP_GIFTS_SYNCHRONIZE);
}

}  // namespace item

}  // namespace server

}  // namespace game

