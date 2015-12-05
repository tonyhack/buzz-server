//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-24 23:16:13.
// File name: task_trip_message_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/task_trip_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "entity/resource_types.h"
#include "entity/task_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/load_trip_configure.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TaskTripMessageHandler::TaskTripMessageHandler() {}
TaskTripMessageHandler::~TaskTripMessageHandler() {}

bool TaskTripMessageHandler::Initialize() {
  // MESSAGE_TASK_TRIP_OPT_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_TRIP_OPT_REQUEST,
    boost::bind(&TaskTripMessageHandler::OnMessageOpt, this, _1, _2, _3));
    return true;
}

void TaskTripMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_TRIP_OPT_REQUEST);
}

void TaskTripMessageHandler::OnMessageOpt(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 获取任务玩家对象
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(id);
  if(task_actor == NULL) {
    LOG_ERROR("任务玩家对象没有找到[%lu]", id);
    return ;
  }

  // 获取玩家游戏对象
  GameActor *game_actor = task_actor->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("玩家对象没有找到[%lu]", id);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskTripOptRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_TRIP_OPT_REQUEST from actor(%lu, %s), type_(%d), value_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.type_, message.value_);

  // 验证功能有没有开启
  if(game_actor->CheckFunctionalityState(
        entity::FunctionalityType::TASK_TRIP) == false) {
    LOG_ERROR("循环任务功能尚未开启[%lu]", id);
    return ;
  }

  // 得到奇遇任务
  TaskTrip &trip = task_actor->GetTrip();
  Task *task = trip.GetTask((core::uint32)message.value_);
  if(task == NULL) {
    LOG_ERROR("奇遇任务没有找到[%lu]", game_actor->GetActorID());
    return ;
  }

  // 检测钻石
  if(entity::TaskTripOptType::SPENT_FINISH == message.type_) {
    // 得到奇遇任务配置
    const TripTaskCell *cell = Configure::GetInstance()->GetTripConditionCell(message.value_);
    if(cell == NULL) {
      LOG_ERROR("奇遇任务配置没有找到");
      return ;
    }
    if(game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < 
        cell->spent_) {
      LOG_ERROR("玩家钻石不足,需要消耗[%d]", cell->spent_);
      return ;
    }
  }

  if(entity::TaskTripOptType::REQUEST_GIVE_UP == message.type_) {
    // 玩家放弃奇遇任务
    if(trip.RemoveTask(message.value_) == false) {
      LOG_ERROR("删除玩家奇遇任务出错[%lu,[%d]]", game_actor->GetActorID(), message.value_);
      return ;
    }
    TaskBuilder::GetInstance()->Destory(task);
    task = NULL;
    trip.SetTaskNum(trip.GetTaskNum() - 1);
  } else if(entity::TaskTripOptType::REQUEST_FINISH == message.type_ ||
      entity::TaskTripOptType::SPENT_FINISH == message.type_) {
    // 玩家请求完成奇遇任务
    const TripTaskCell *cell = Configure::GetInstance()->GetTripConditionCell(message.value_);
    if(cell == NULL) {
      LOG_ERROR("奇遇任务配置没有找到");
      return ;
    }
    if(entity::TaskTripOptType::SPENT_FINISH != message.type_) {
      if(task->CheckCondition() == false) {
        LOG_ERROR("玩家[%lu]奇遇任务[%d]条件没有达成", game_actor->GetActorID(),
            message.value_);
        return ;
      }
    }
    if(FacadeRequest::GetInstance()->Awards(id, cell->awards_) == false) {
      LOG_ERROR("添加奇遇任务奖励失败");
      return ;
    }
    if(entity::TaskTripOptType::SPENT_FINISH == message.type_) {
      game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - cell->spent_);
    }

    // 删除任务
    if(trip.RemoveTask(message.value_) == false) {
      LOG_ERROR("玩家[%lu]删除任务[%d]出错", game_actor->GetActorID(), message.value_);
      return ;
    }
    TaskBuilder::GetInstance()->Destory(task);   
  }

  gateway::protocol::MessageTaskTripResponse response;
  response.__set_type_(message.type_);
  response.__set_result_(message.value_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_TRIP_OPT_RESPONSE);
}

} // namespace task

} // namespace server

} // namespace game

