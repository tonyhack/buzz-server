//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-24 01:13:21.
// File name: actor_attr_event_handler.cc
//
// Description:
// Define class ActorAttrEventHandler.
//

#include "game_server/server/extension/task/actor_attr_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace task {

ActorAttrEventHandler::ActorAttrEventHandler() {}
ActorAttrEventHandler::~ActorAttrEventHandler() {}

bool ActorAttrEventHandler::Initialize() {
  // 玩家升级
  this->conn_level_up_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_LEVEL_UP,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorAttrEventHandler::OnEventActorLevelUp, this, _1, _2, _3, _4));

  // 资源变化
  this->conn_resource_change_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_RESOURCE_CHANGE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorAttrEventHandler::OnEventActorResourceChange, this, _1, _2, _3, _4));

 return true;
}

void ActorAttrEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_resource_change_);
}

void ActorAttrEventHandler::OnEventActorLevelUp(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLevelUp) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventActorLevelUp *event = (event::EventActorLevelUp *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->actor_id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家[%lu]对象没有找到", event->actor_id_);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_ACTOR_LEVEL_UP, message, size);

/*
  if(event->new_level_ >= entity::g_constants_constants.kMaxCircleLimitLevel &&
      event->old_level_ < entity::g_constants_constants.kMaxCircleLimitLevel) {
    if(game_actor->CheckFunctionalityState(entity::FunctionalityType::TASK_CIRCLE) == false) {
      game_actor->SetFunctionalityState(entity::FunctionalityType::TASK_CIRCLE);
    }
    // 初始化循环任务
    TaskCircle& circle = actor->GetCircle();
    circle.InitCircle(1);
    circle.RoundSynchronize();
  }
*/

}

void ActorAttrEventHandler::OnEventActorResourceChange(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorResourceChange) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventActorResourceChange *event = (event::EventActorResourceChange *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家[%lu]对象没有找到", event->actor_);
    return ;
  }

  // 处理任务数据
  actor->OnEvent(event::EventType::EVENT_ACTOR_RESOURCE_CHANGE, message, size);

  if(event->type_ == entity::ResourceID::HONOUR) {
    // 军功属性变化
    actor->GetTaskDaily().SumAttr(event->old_num_, false, true);
    actor->GetTaskDaily().SumAttr(event->new_num_, true, true);
  }
}

}  // namespace task

}  // namespace server

}  // namespace game
