
#include "game_server/server/extension/task/task_homeland_event_handle.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/event/game_social_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace task {

TaskHomelandEventHandler::TaskHomelandEventHandler() {}
TaskHomelandEventHandler::~TaskHomelandEventHandler() {}

bool TaskHomelandEventHandler::Initialize() {
  this->conn_watering_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_SOCIAL_HOMELAND_TREE_WATER,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&TaskHomelandEventHandler::OnEventWatering, this, _1, _2, _3, _4));

  this->conn_rob_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&TaskHomelandEventHandler::OnEventRob, this, _1, _2, _3, _4));

 return true;
}

void TaskHomelandEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_rob_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_watering_);
}

void TaskHomelandEventHandler::OnEventWatering(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSocialHomeLandTreeWater) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSocialHomeLandTreeWater *event = (event::EventSocialHomeLandTreeWater *)message;

  // 得到任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 找不到玩家对象(%u)",
        __FILE__, __LINE__, __FUNCTION__, channel);
    return ;
  }
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家[%lu]对象没有找到", event->id_);
    return ;
  }

  // 事件回调处理
  actor->OnEvent(event::EventType::EVENT_SOCIAL_HOMELAND_TREE_WATER, message, size);
}

void TaskHomelandEventHandler::OnEventRob(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventSocialHomeLandBattleStrike) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventSocialHomeLandBattleStrike *event = (event::EventSocialHomeLandBattleStrike *)message;

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
  actor->OnEvent(event::EventType::EVENT_SOCIAL_HOMELAND_BATTLE_STRIKE, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game
