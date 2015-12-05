//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-16 18:04:52.
// File name: soul_appear_event_handler.cc
//
// Description:
// Define class SoulAppearEventHandler.
//

#include "game_server/server/extension/task/soul_appear_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_skill_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace task {

SoulAppearEventHandler::SoulAppearEventHandler() {}
SoulAppearEventHandler::~SoulAppearEventHandler() {}

bool SoulAppearEventHandler::Initialize() {
  // Follow 所有玩家使用幻化的事件
  this->connection_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SKILL_USE_APPEAR, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&SoulAppearEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void SoulAppearEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->connection_);
}

void SoulAppearEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventUseAppearSkill) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventUseAppearSkill *event = (event::EventUseAppearSkill *)message;

  // 任务玩家扩展是否存在
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 任务玩家对象不存在",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_SKILL_USE_APPEAR, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game

