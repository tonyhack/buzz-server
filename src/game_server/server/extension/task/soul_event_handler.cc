//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 19:44:40.
// File name: soul_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/soul_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_soul_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

SoulEventHandler::SoulEventHandler() {}
SoulEventHandler::~SoulEventHandler() {}

bool SoulEventHandler::Initialize() {
  this->conn_soul_set_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SOUL_SET, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SoulEventHandler::OnEventSoulSet,
        this, _1, _2, _3, _4));

  this->conn_soul_upgrade_step_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SOUL_UPGRADE_STEP, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SoulEventHandler::OnEventSoulUpgradeStep,
        this, _1, _2, _3, _4));

  return true;
}

void SoulEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_soul_set_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_soul_upgrade_step_);
}

void SoulEventHandler::OnEventSoulSet(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSoulSet) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSoulSet *event = (event::EventSoulSet *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_SOUL_SET, message, size);
}

void SoulEventHandler::OnEventSoulUpgradeStep(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSoulUpgradeStep) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventSoulUpgradeStep *event = (event::EventSoulUpgradeStep *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, event->actor_);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_SOUL_UPGRADE_STEP, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game

