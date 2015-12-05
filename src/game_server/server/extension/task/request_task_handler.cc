//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-23 19:47:03.
// File name: request_task_handler.cc
//
// Description:
//
#include "game_server/server/extension/task/request_task_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/request/game_task_request_types.h"
#include "global/configure/configure.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

RequestTaskHandler::RequestTaskHandler() {}
RequestTaskHandler::~RequestTaskHandler() {}

bool RequestTaskHandler::Initialize() {
  // REQUEST_TASK_ACCEPT
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TASK_ACCEPT,
      boost::bind(&RequestTaskHandler::OnRequest, this, _1, _2));
  // REQUEST_TASK_CHECK_GATHER
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TASK_CHECK_GATHER,
      boost::bind(&RequestTaskHandler::OnCheckRequest, this, _1, _2));
  // REQUEST_TASK_CONDITION_COORDINATE
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TASK_CONDITION_COORDINATE,
      boost::bind(&RequestTaskHandler::OnRequestTaskConditionCoordinate, this, _1, _2));
  // REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE,
      boost::bind(&RequestTaskHandler::OnRequestTaskAcceptAndFinishCoordinate, this, _1, _2));

  return true;
}

void RequestTaskHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TASK_ACCEPT);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TASK_CHECK_GATHER);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TASK_CONDITION_COORDINATE);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_TASK_ACCEPT_AND_FINISH_COORDINATE);
}

int RequestTaskHandler::OnRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTaskAccept) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestTaskAccept *request = (request::RequestTaskAccept *)message;

  // 得到玩家任务
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request->actor_id_);
  if(task_actor == NULL) {
    LOG_ERROR("玩家的任务对象没有找到");
    return -1;
  }
  gateway::protocol::MessageTaskAccept response;
  response.__set_task_type_(request->type_);

  if(request->type_ == entity::TaskType::TASK_RISK) {
    // 得到配置
    const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(
        (core::uint32)request->task_id_);
    if(cell == NULL) {
      LOG_ERROR("任务配置没有找到[%d]", request->task_id_);
      return -1;
    }

    // 检测该任务是否已完成
    if(task_actor->CheckTaskFinish(request->task_id_) == true) {
      global::LogError("%s:%d (%s) 玩家请求接受任务时，该任务已完成",
          __FILE__, __LINE__, __FUNCTION__);
      ErrorCode::Send(task_actor->GetID(),entity::ErrorType::TASK_ACCPED_RISK);
      return -1;
    }

    // 任务是否已接受
    if(task_actor->GetTask(request->task_id_) != NULL) {
      global::LogError("%s:%d (%s) 玩家请求接受任务时，该任务已在任务列表中",
          __FILE__, __LINE__, __FUNCTION__);
      ErrorCode::Send(task_actor->GetID(),entity::ErrorType::TASK_ACCPED_RISK);
      return -1;
    }

    // 创建一个任务对象
    Task *task = TaskBuilder::GetInstance()->Create(task_actor,
        entity::TaskType::TASK_MAIN, request->task_id_);
    if(task == NULL) {
      global::LogError("%s:%d (%s) 接受任务时，创建任务对象失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }

    // 增加到任务列表
    if(task_actor->AddTask(task) == false) {
      TaskBuilder::GetInstance()->Destory(task);
      global::LogError("%s:%d (%s) 接受任务时，增加任务到玩家任务列表失败",
          __FILE__, __LINE__, __FUNCTION__);
      return -1;
    }
    response.__set_id_(request->task_id_);
  } else if(request->type_ == entity::TaskType::TASK_TRIP) {
    // 检测配置
    TaskTrip &trip = task_actor->GetTrip();
    if(trip.GetTaskNum() >= 10) {
      LOG_ERROR("超过每天最大数目[%d]", trip.GetTaskNum());
      return -1;
    }
    // 获取玩家等级段
    GameActor *game_actor = task_actor->GetActor();
    if(game_actor == NULL) {
      LOG_ERROR("玩家对象没有找到");
      return -1;
    }
    core::int32 level = game_actor->GetAttribute(entity::RoleAoiFields::LEVEL);
    // 随机配置
    const LoadTripConfigure::RandConditionVec *conditions = 
      Configure::GetInstance()->GetRandCondition(level/entity::g_constants_constants.kMaxTripLevelStage - 
          entity::g_constants_constants.kMaxTripLevelStageStart, request->task_id_);
    if(conditions == NULL) {
      LOG_ERROR("当前的配置没有找到level_stage[%d]type[%d]", level/entity::g_constants_constants.kMaxTripLevelStage -
          entity::g_constants_constants.kMaxTripLevelStageStart, request->task_id_);
      return -1;
    }
    core::int32 rand_value = ExtensionManager::GetInstance()->GetRandom().Random(conditions->size());
    if(rand_value  >  (core::int32)conditions->size()) {
      return -1;
    }
    const LoadTripConfigure::RandConditionVec& conditions_temp = *conditions;
    if(trip.CheckTaskAccept(conditions_temp[rand_value].order_id_) == false) {
      LOG_ERROR("该任务ID不能被添加到奇遇任务列表");
      ErrorCode::Send(task_actor->GetID(),entity::ErrorType::TASK_ACCPED_SAME_TRIP);
      return -1;
    }
    // 添加
    Task *task = TaskBuilder::GetInstance()->Create(task_actor,
        entity::TaskType::TASK_TRIP, conditions_temp[rand_value].order_id_);
    if(task == NULL) {
      LOG_ERROR("创建奇遇任务错误");
      return -1;
    }
    if(trip.AddTask(task) == false) {
      LOG_ERROR("添加奇遇任务错误");
      TaskBuilder::GetInstance()->Destory(task);
      return -1;
    }
    response.__set_id_(conditions_temp[rand_value].order_id_);
    trip.SetTaskNum(trip.GetTaskNum() + 1);
  }
  // 发送到客户端
  task_actor->SendMessage(response, gateway::protocol::MessageType::MESSAGE_TASK_ACCEPT);
  return 0;
}

int RequestTaskHandler::OnCheckRequest(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTaskCheckGather) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestTaskCheckGather *request = (request::RequestTaskCheckGather *)message;

  // 得到玩家任务
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request->actor_id_);
  if(task_actor == NULL) {
    LOG_ERROR("玩家的任务对象没有找到");
    return -1;
  }

  // 检查
  if(task_actor->CheckGatherTask(request->npc_template_) == false) {
    LOG_ERROR("检查任务中的采集失败");
    return -1;
  }
  return 0;
}

int RequestTaskHandler::OnRequestTaskConditionCoordinate(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTaskConditionCoordinate) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestTaskConditionCoordinate *request = (request::RequestTaskConditionCoordinate *)message;

  // 得到玩家任务
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request->actor_id_);
  if(task_actor == NULL) {
    LOG_ERROR("玩家的任务对象没有找到");
    return -1;
  }

  // 得到任务
  Task *task = task_actor->GetTask(request->type_, request->task_id_);
  if(task == NULL) {
    LOG_ERROR("任务(%d)type(%d)没有找到", request->task_id_, request->type_);
    return -1;
  }

  // 得到坐标
  if(task->GetCoordinate(Task::CONDITION, request->condition_pos_, request->map_id_, request->x_,
        request->y_) == false) {
    LOG_ERROR("获取任务(%d)条件(%d)坐标失败", request->task_id_, request->condition_pos_);
    return -1;
  }
  return 0;
}

int RequestTaskHandler::OnRequestTaskAcceptAndFinishCoordinate(void *message, size_t size) {
  if(message == NULL || sizeof(request::RequestTaskAcceptAndFinishCoordinate) != size) {
    global::LogError("%s:%d (%s) 请求增加道具时，参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }

  request::RequestTaskAcceptAndFinishCoordinate *request = (request::RequestTaskAcceptAndFinishCoordinate *)message;

  // 得到玩家任务
  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(request->actor_id_);
  if(task_actor == NULL) {
    LOG_ERROR("玩家的任务对象没有找到");
    return -1;
  }

  if(request->type_ == entity::TaskType::TASK_MAIN || request->type_ ==
      entity::TaskType::TASK_RISK) {
    const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(request->task_id_);
    if(cell == NULL) {
      LOG_ERROR("任务配置(%d)没有找到", request->task_id_);
      return -1;
    }
    if(request->opt_type_ == request::RequestTaskOptType::ACCEPT) {
      if(task_actor->CheckTaskAccept(request->task_id_) == false) {
        return -1;
      } else {
        request->map_id_ = cell->accept_coordinate_.map_;
        request->x_ = cell->accept_coordinate_.x_;
        request->y_ = cell->accept_coordinate_.y_;
      }
    }
    if(request->opt_type_ == request::RequestTaskOptType::FINISH) {
      request->map_id_ = cell->finish_coordinate_.map_;
      request->x_ = cell->finish_coordinate_.x_;
      request->y_ = cell->finish_coordinate_.y_;
    }
    return 0;
  } else if(request->type_ == entity::TaskType::TASK_TRANSPORT) {
    request->x_ = MISC_CONF()->transport_task_accept_npc_x_;
    request->y_ = MISC_CONF()->transport_task_accept_npc_y_;
    request->map_id_ = MISC_CONF()->transport_task_accept_npc_map_;
    return 0;
  }

  return -1;
}

}  // namespace task

}  // namespace server

}  // namespace game

