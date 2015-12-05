
#include "game_server/server/extension/welfare/item_event_handler.h"

#include <boost/bind.hpp>
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

ItemEventHandler::ItemEventHandler() {}

ItemEventHandler::~ItemEventHandler() {}

bool ItemEventHandler::Initialize() {
  this->conn_equip_strength_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ITEM_EQUIP_STRENGTH,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ItemEventHandler::OnEventEquipStrength, this, _1, _2, _3, _4));

  this->conn_equip_wear_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ITEM_WEAR_EQUIP,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ItemEventHandler::OnEventEquipWear, this, _1, _2, _3, _4));

  this->conn_use_item_ = 
      coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ITEM_USE,
      coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ItemEventHandler::OnEventUseItem, this, _1, _2, _3, _4));

  return true;
}

void ItemEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_equip_wear_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_use_item_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_equip_strength_);
}

void ItemEventHandler::OnEventEquipStrength(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemEquipStrength) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemEquipStrength *event = (event::EventItemEquipStrength *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
  if(actor->GetAchieve() == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }

  actor->GetAchieve()->OnEvent(event::EventType::EVENT_ITEM_EQUIP_STRENGTH, message, size);
}

void ItemEventHandler::OnEventEquipWear(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemWearEquip) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemWearEquip *event = (event::EventItemWearEquip *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(event->id_);
  if(actor->GetAchieve() == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }

  actor->GetAchieve()->OnEvent(event::EventType::EVENT_ITEM_WEAR_EQUIP, message, size);
}

void ItemEventHandler::OnEventUseItem(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemUse) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemUse *event = (event::EventItemUse *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor->GetAchieve() == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }

  actor->GetAchieve()->OnEvent(event::EventType::EVENT_ITEM_USE, message, size);
}

}  // namespace welfare

}  // namespace server

}  // namespace game
