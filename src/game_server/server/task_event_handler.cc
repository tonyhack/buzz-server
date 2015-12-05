//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-05 19:41:47.
// File name: task_event_handler.cc
//
// Description:
//
#include "game_server/server/task_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/constants_constants.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_task_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/load_functionality_configure.h"

namespace game {

namespace server {

TaskEventHandler::TaskEventHandler() {}
TaskEventHandler::~TaskEventHandler() {}

bool TaskEventHandler::Initialize() {
  // EVENT_TASK_ACCEPT
  this->conn_accept_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_TASK_ACCEPT,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&TaskEventHandler::OnAccept, this, _1, _2, _3, _4));
  // EVENT_TASK_FINISH
  this->conn_finish_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_TASK_FINISH,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&TaskEventHandler::OnFinish, this, _1, _2, _3, _4));

  return true;
}

void TaskEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_accept_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_finish_);
}

void TaskEventHandler::OnAccept(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventTaskAccept) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventTaskAccept *event = (event::EventTaskAccept *)message;
  LoadFunctionalityConfigure::OpenCells& cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::TASK_ACCEPT);
  
  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", event->actor_id_);
    return ;
  }

  // 开启功能
  for(core::uint32 i = 0; i < cells.size(); ++i) {
    if(cells[i].param_ == event->task_id_ &&
        game_actor->CheckFunctionalityState(cells[i].function_) == false) {
      game_actor->SetFunctionalityState(cells[i].function_, true, true);
    }
  }
}

void TaskEventHandler::OnFinish(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventTaskFinish) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return ;
  }

  event::EventTaskFinish *event = (event::EventTaskFinish *)message;

  if(event->type_ != entity::TaskType::TASK_MAIN) {
    return ;
  }

  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->actor_id_);
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", event->actor_id_);
    return ;
  }

  // 开启功能
  LoadFunctionalityConfigure::OpenCells& cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::TASK_FINISH);
  for(core::uint32 i = 0; i < cells.size(); ++i) {
    if(cells[i].param_ == event->task_id_ &&
        game_actor->CheckFunctionalityState(cells[i].function_) == false) {
      game_actor->SetFunctionalityState(cells[i].function_, true, true);
    }
  }
}

} // namespace server

} // namepsace game
