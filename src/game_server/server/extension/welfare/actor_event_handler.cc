
#include "game_server/server/extension/welfare/actor_event_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_scene_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/welfare/achieve_actor.h"
#include "game_server/server/extension/welfare/welfare_actor.h"
#include "game_server/server/extension/welfare/welfare_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace welfare {

ActorEventHandler::ActorEventHandler() {}

ActorEventHandler::~ActorEventHandler() {}

bool ActorEventHandler::Initialize() {
  this->conn_relive_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_RELIVE, coresh::Communicator::GetChannelAll(), 
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
      &ActorEventHandler::OnEventRelive, this, _1, _2, _3, _4));

  this->conn_scene_num_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SCENE_JUMP_MAP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
      &ActorEventHandler::OnEventSceneNum, this, _1, _2, _3, _4));

  this->conn_resource_change_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_RESOURCE_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
      &ActorEventHandler::OnEventResourceChange, this, _1, _2, _3, _4));

  this->conn_vip_status_change_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_VIP_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
      &ActorEventHandler::OnEventVipStatusChange, this, _1, _2, _3, _4));

  this->conn_funcationality_open_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(
      &ActorEventHandler::OnEventFuncationalityOpen, this, _1, _2, _3, _4));

  return true;
}

void ActorEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_relive_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_scene_num_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_resource_change_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_vip_status_change_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_funcationality_open_);
}

void ActorEventHandler::OnEventRelive(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorRelive) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventActorRelive *event = (event::EventActorRelive *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }

  AchieveActor *achieve_actor = actor->GetAchieve();

  achieve_actor->OnEvent(event::EventType::EVENT_ACTOR_RELIVE, message, size);
}

void ActorEventHandler::OnEventSceneNum(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSceneJumpMap) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSceneJumpMap *event = (event::EventSceneJumpMap *)message;

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }
  AchieveActor *achieve_actor = actor->GetAchieve();

  achieve_actor->OnEvent(event::EventType::EVENT_SCENE_JUMP_MAP, message, size);
}

void ActorEventHandler::OnEventResourceChange(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorResourceChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }
  AchieveActor *achieve_actor = actor->GetAchieve();

  achieve_actor->OnEvent(event::EventType::EVENT_ACTOR_RESOURCE_CHANGE, message, size);
}

void ActorEventHandler::OnEventVipStatusChange(core::uint64 channel, core::uint8 channel_type, 
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorVipChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }
  AchieveActor *achieve_actor = actor->GetAchieve();

  achieve_actor->OnEvent(event::EventType::EVENT_ACTOR_VIP_CHANGE, message, size);
}

void ActorEventHandler::OnEventFuncationalityOpen(core::uint64 channel, core::uint8 channel_type, 
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorFunctionalityEnable) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  // 获取玩家对象
  WelfareActor *actor = WelfareActorManager::GetInstance()->Get(channel);
  if(actor == NULL) {
    LOG_ERROR("玩家(%lu)没有找到", channel);
    return ;
  }
  AchieveActor *achieve_actor = actor->GetAchieve();

  achieve_actor->OnEvent(event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, message, size);
}

}  // namesapce welfare

}  // namespace server

}  // namespace game
