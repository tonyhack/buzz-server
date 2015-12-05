//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-16 00:13:40.
// File name: functionality_event_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/functionality_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/event/game_actor_event_types.h"

namespace game {

namespace server {

namespace task {

FunctionalityEventHandler::FunctionalityEventHandler() {}
FunctionalityEventHandler::~FunctionalityEventHandler() {}

bool FunctionalityEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_functionality_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&FunctionalityEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void FunctionalityEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_functionality_);
}

void FunctionalityEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorFunctionalityEnable) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorFunctionalityEnable *event = (event::EventActorFunctionalityEnable *)message;
  
  // 从玩家管理器中获取玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 没有找到，获取 GameActor 失败",
        __FILE__, __LINE__, __FUNCTION__, event->actor_id_);
    return ;
  }

  if(event->functionality_id_ == entity::FunctionalityType::TASK_CIRCLE) {
    // 任务玩家扩展是否存在
    TaskActor *task_actor = TaskActorManager::GetInstance()->Get(
        event->actor_id_);
    if(task_actor == NULL) {
      global::LogError("%s:%d (%s) 玩家(%lu) 任务扩展已存在",
          __FILE__, __LINE__, __FUNCTION__, event->actor_id_);
      return ;
    }

    TaskCircle& circle = task_actor->GetCircle();
    circle.InitCircle(1);
    circle.RoundSynchronize();
  } else if(event->functionality_id_ == entity::FunctionalityType::TASK_RISK) {
    // 处理历险任务

  } else if(event->functionality_id_ == entity::FunctionalityType::TASK_TRIP) {
    // 处理奇遇任务
  }
}

}  // namespace task

}  // namespace server

}  // namespace game

