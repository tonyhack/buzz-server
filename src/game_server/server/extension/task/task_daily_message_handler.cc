#include "game_server/server/extension/task/task_daily_message_handler.h"

#include <boost/bind.hpp>

#include "entity/constants_constants.h"
#include "entity/resource_types.h"
#include "entity/task_types.h"
#include "game_server/server/extension_manager.h"
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

TaskDailyMessageHandler::TaskDailyMessageHandler() {}
TaskDailyMessageHandler::~TaskDailyMessageHandler() {}

bool TaskDailyMessageHandler::Initialize() {
  // MESSAGE_TASK_DAILY_FINISH_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_DAILY_REQUEST,
    boost::bind(&TaskDailyMessageHandler::OnTask, this, _1, _2, _3));
  // MESSAGE_TASK_DAILY_AWARD_REQUEST
  ExtensionManager::GetInstance()->SetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_DAILY_AWARD_REQUEST,
    boost::bind(&TaskDailyMessageHandler::OnAward, this, _1, _2, _3));

    return true;
}

void TaskDailyMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_DAILY_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
    gateway::protocol::MessageType::MESSAGE_TASK_DAILY_AWARD_REQUEST);
}

void TaskDailyMessageHandler::OnTask(core::uint64 id,
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
  gateway::protocol::MessageTaskDailyRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_DAILY_FINISH_REQUEST from actor(%lu, %s), id_(%d), opt_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_, message.opt_);

  // 得到每日任务
  TaskDaily &task_daily = task_actor->GetTaskDaily();

  if(message.opt_ == 0) {
    // 请求任务
    if(task_daily.CheckTaskAccept(message.id_) == false) {
      LOG_ERROR("此每日任务(%d)不可接取", message.id_);
      return ;
    }
    // 加入任务列表
    Task *task = TaskBuilder::GetInstance()->Create(task_actor, entity::TaskType::TASK_DAILY, message.id_);
    if(task == NULL) {
      LOG_ERROR("创建每日任务(%d)失败", message.id_);
      return ;
    }
    task_daily.AddTask(task);
  }  else  if (message.opt_ == 1){
    // 请求完成任务
    if(task_daily.CheckTaskFinish(message.id_) == false) {
      LOG_ERROR("每日任务(%d)没有完成", message.id_);
      return ;
    }
    // 完成任务
    if(task_daily.FinishTask(message.id_) == false) {
      LOG_ERROR("每日任务完成出现问题");
      return ;
    }
  } else {
    // 扣除快速完成消耗
    const TaskDailyCell *cell = Configure::GetInstance()->GetTaskDailyCell(message.id_);
    if(cell == NULL) {
      LOG_ERROR("任务配置没有找到(%d)", message.id_);
      return ;
    }
    if(game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) < cell->spend_) {
      LOG_ERROR("快速完成每日任务(%d),钻石不足(%d)", message.id_, cell->spend_);
      return ;
    }
    // 完成任务
    if(task_daily.FinishTask(message.id_) == false) {
      LOG_ERROR("每日任务完成出现问题");
      return ;
    }
  }
  // 返回客户端
  gateway::protocol::MessageTaskDailyResponse response;
  response.__set_id_(message.id_);
  response.__set_opt_(message.opt_);
  game_actor->SendMessage(response, 
      gateway::protocol::MessageType::MESSAGE_TASK_DAILY_RESPONSE);
}

void TaskDailyMessageHandler::OnAward(core::uint64 id, 
    const char *data, size_t size) {
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
  gateway::protocol::MessageTaskDailyAwardRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_DAILY_AWARD_REQUEST from actor(%lu, %s), id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_);

  // 得到每日任务
  TaskDaily &task_daily = task_actor->GetTaskDaily();

  // 检查是否可领取
  if(task_daily.CheckDailyAward(message.id_) == false) {
    LOG_ERROR("不可领取(%d)", message.id_);
    return ;
  }

  const ExploitAwardCell *cell = Configure::GetInstance()->GetTaskDailyAwardCell(message.id_);
  if(cell == NULL) {
    LOG_ERROR("配置没有找到(%d)", message.id_);
    return ;
  }

  if(FacadeRequest::GetInstance()->Awards(game_actor->GetActorID(), cell->award_, 5) == false) {
    LOG_ERROR("玩家(%lu) 增加任务奖励失败",game_actor->GetActorID());
    return ;
  }

  task_daily.SetPicked(message.id_);
  gateway::protocol::MessageTaskDaliyAwardResponse response;
  response.__set_id_(message.id_);
  game_actor->SendMessage(response, 
      gateway::protocol::MessageType::MESSAGE_TASK_DAILY_AWARD_RESPONSE);

}

} // namespace task

} // namespace server

} // namespace game

