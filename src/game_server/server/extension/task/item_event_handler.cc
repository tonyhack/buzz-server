//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 22:48:38.
// File name: item_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/item_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_item_event_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

ItemEventHandler::ItemEventHandler() {}
ItemEventHandler::~ItemEventHandler() {}

bool ItemEventHandler::Initialize() {
  // Follow 关注道具使用事件
  this->conn_item_use_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_USE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnUseEvent,
        this, _1, _2, _3, _4));
  // Follow 关注宝石合成事件
  this->conn_equip_inlay_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_EQUIP_INLAY, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnInlayEvent,
        this, _1, _2, _3, _4));

  // Follow 关注装备洗练事件
  this->conn_equip_refine_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_EQUIP_REFINE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnRefineEvent,
        this, _1, _2, _3, _4));
  // Follow 关注装备强化事件
  this->conn_equip_strength_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_EQUIP_STRENGTH, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnStrengthEvent,
        this, _1, _2, _3, _4));
  // Follow 关注占星事件
  this->conn_divine_gem_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_DIVINE_GEM, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnDivineGemEvent,
        this, _1, _2, _3, _4));
  // Follow 关注穿装备事件
  this->conn_wear_equip_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ITEM_WEAR_EQUIP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ItemEventHandler::OnWearEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ItemEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_equip_strength_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_equip_refine_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_equip_inlay_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_item_use_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_divine_gem_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_wear_equip_);
}

void ItemEventHandler::OnUseEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemUse) != size ||
       channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }
  
  event::EventItemUse *event = (event::EventItemUse *)message;
  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_USE, message, size);

}

void ItemEventHandler::OnRefineEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemEquipRefine) != size ||
       channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemEquipRefine *event = (event::EventItemEquipRefine *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_EQUIP_REFINE, message, size);
}

void ItemEventHandler::OnStrengthEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemEquipStrength) != size ||
       channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }
  
  event::EventItemEquipStrength *event = (event::EventItemEquipStrength *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_EQUIP_STRENGTH, message, size);
}

void ItemEventHandler::OnInlayEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemEquipInlay) != size ||
       channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }
  
  event::EventItemEquipInlay *event = (event::EventItemEquipInlay *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_EQUIP_INLAY, message, size);
}


void ItemEventHandler::OnDivineGemEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemDivineGem) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemDivineGem *event = (event::EventItemDivineGem *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_DIVINE_GEM, message, size);
}

void ItemEventHandler::OnWearEvent(core::uint64 channel, core::uint8 channel_type,
        const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventItemWearEquip) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventItemWearEquip *event = (event::EventItemWearEquip *)message;
  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ITEM_WEAR_EQUIP, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game

