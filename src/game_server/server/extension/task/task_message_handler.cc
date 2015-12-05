//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-06 14:17:35.
// File name: task_message_handler.cc
//
// Description:
// Define class TaskMessageHandler.
//

#include "game_server/server/extension/task/task_message_handler.h"

#include <boost/bind.hpp>

#include "entity/resource_types.h"
#include "entity/task_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/extension/task/load_summon_npc_configure.h"
#include "game_server/server/extension/task/task.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_builder.h"
#include "game_server/server/extension/task/task_configure.h"
#include "game_server/server/extension/task/task_extension.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_task_event_types.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/distance_calculator.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TaskMessageHandler::TaskMessageHandler() {}
TaskMessageHandler::~TaskMessageHandler() {}

bool TaskMessageHandler::Initialize() {
  // 玩家任务操作
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_ACCEPT,
      boost::bind(&TaskMessageHandler::OnMessageAcceptTask, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_FINISH,
      boost::bind(&TaskMessageHandler::OnMessageFinishTask, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TALK_NPC,
      boost::bind(&TaskMessageHandler::OnMessageTalkNpc, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_ABANDON,
      boost::bind(&TaskMessageHandler::OnMessageAbandonTask, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_SUMMON_NPC_REQUEST,
      boost::bind(&TaskMessageHandler::OnMessageSummonNpc, this, _1, _2, _3));

  return true;
}

void TaskMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_ACCEPT);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_FINISH);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TALK_NPC);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_ABANDON);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_SUMMON_NPC_REQUEST);
}

void TaskMessageHandler::OnMessageAcceptTask(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskAccept message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_ACCEPT from actor(%lu, %s), id_(%d), npc_(%s), task_type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_, message.npc_.c_str(), message.task_type_);

  // 得到配置信息
  const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(message.id_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 接受了一个不存在的任务ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, id, message.id_);
    return ;
  }

  if(actor->CheckTaskAccept(message.id_) == false) {
    LOG_ERROR("任务不可接受");
    return ;
  }

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_.c_str()));
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 接受任务时，找不到对应npc对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 是否是接受任务的NPC
  if(npc->GetTemplateID() != cell->accept_npc_) {
    global::LogError("%s:%d (%s) 接受任务时，该NPC不可接受对应任务",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    global::LogError("%s:%d (%s) 接受任务时，与NPC距离过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 添加到任务列表中
  Task *task = actor->AddTaskById(message.id_);
  if(task == NULL) {
    LOG_ERROR("主线任务[%d]没有添加到玩家[lu]主线任务列表中",
        message.id_, id);
    return ;
  }

  // 发送到客户端
  message.__set_task_type_(entity::TaskType::TASK_MAIN);
  actor->SendMessage(
      message, gateway::protocol::MessageType::MESSAGE_TASK_ACCEPT);

  // 广播接取任务事件
  event::EventTaskAccept event;
  event.__set_actor_id_(id);
  event.__set_task_id_(message.id_);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_TASK_ACCEPT, id, 
      event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}

void TaskMessageHandler::OnMessageFinishTask(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskFinish message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_FINISH from actor(%lu, %s), id_(%d), npc_(%s), task_type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_, message.npc_.c_str(), message.task_type_);

  // 得到配置信息
  const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(message.id_);
  if(cell == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 要求完成一个不存在的任务ID(%u)",
        __FILE__, __LINE__, __FUNCTION__, id, message.id_);
    return ;
  }

  // 得到相应的任务对象
  Task *task = actor->GetTask(message.id_);
  if(task == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 要求完成任务列表中不存在的任务(%u)",
        __FILE__, __LINE__, __FUNCTION__, id, message.id_);
    return ;
  }

  // 检测任务是否完成
  if(task->CheckCondition() == false) {
    global::LogError("%s:%d (%s) 玩家(%lu) 要求完成的任务(%u)，不满足完成条件",
        __FILE__, __LINE__, __FUNCTION__, id, message.id_);
    return ;
  }

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_.c_str()));
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 完成任务时，找不到对应npc对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 是否是接受任务的NPC
  if(npc->GetTemplateID() != cell->finish_npc_) {
    global::LogError("%s:%d (%s) 完成任务时，该NPC不可完成对应任务",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    global::LogError("%s:%d (%s) 完成任务时，与NPC距离过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 防沉迷检查
  if(game_actor->GetAttribute(entity::ActorClientFields::FCM_STATUS) ==
     entity::FcmStatusType::ZERO_INCOME) {
    LOG_ERROR("玩家处于沉迷零收益状态，无法提交任务");
    return;
  }

  // 奖励可选道具
  if(message.reward_id_ >= 0 && message.reward_id_ < 5 &&
      cell->award_item_[message.reward_id_].id_ > 0) {
    if(FacadeRequest::GetInstance()->Awards(id, &cell->award_item_[message.reward_id_], 1) == false) {
      LOG_ERROR("加入可先奖励时出错");
      return ;
    }
  }

  // 加入奖励
  if(FacadeRequest::GetInstance()->Awards(id, cell->award_, 5) == false) {
    LOG_ERROR("加入任务奖励时出错");
    return ;
  }
 
  // 在删除任务前对任务处理
  task->Clear();
  task->OnFinish();
  // 完成任务时从任务列表中删除
  task = actor->RemoveTask(message.id_);
  if(task == NULL) {
    global::LogError("%s:%d (%s)) 玩家(%lu) 从任务列表中删除任务失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }
  TaskBuilder::GetInstance()->Destory(task);

  // 加入完成列表
  actor->FinishTask(message.id_);
  actor->AddLayer(message.id_);

  // 发送到客户端
  actor->SendMessage(message, gateway::protocol::MessageType::MESSAGE_TASK_FINISH);

  // 广播任务完成事件
  event::EventTaskFinish event;
  event.__set_actor_id_(id);
  event.__set_task_id_(message.id_);
  event.__set_type_(entity::TaskType::TASK_MAIN);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_TASK_FINISH, id, 
      event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}

void TaskMessageHandler::OnMessageAbandonTask(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskAbandon message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_ABANDON from actor(%lu, %s), id_(%d), task_type_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.id_, message.task_type_);

  // 得到配置信息
  const TaskCell *cell = Configure::GetInstance()->GetTaskConfigure(message.id_);
  if(cell == NULL) {
    LOG_ERROR("玩家[%lu]任务[%d]配置没有找到", id, message.id_);
    return ;
  }

  if(cell->can_giveup_ == 0) {
    LOG_ERROR("任务[%d]不可放弃", message.id_);
    return ;
  }

  // 得到相应的任务对象
  Task *task = actor->RemoveTask(message.id_);
  if(task == NULL) {
    LOG_ERROR("玩家[%lu]删除任务[%d]失败");
    return ;
  } else {
    task->Clear();
    TaskBuilder::GetInstance()->Destory(task);
    task = NULL;
  }

  // 返回给客户端
  gateway::protocol::MessageTaskAbandon response;
  response.__set_id_(message.id_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_ABANDON);

}

void TaskMessageHandler::OnMessageTalkNpc(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskTalkNpc message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_TALK_NPC from actor(%lu, %s), npc_(%s), task_id_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.npc_.c_str(), message.task_id_);

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_.c_str()));
  if(npc == NULL) {
    global::LogError("%s:%d (%s) 与NPC对话时，找不到对应npc对象",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosx(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    global::LogError("%s:%d (%s) 与NPC对话时，与NPC距离过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 对话NPC事件
  event::EventActorTalkNpc event;
  event.__set_actor_id_(id);
  event.__set_npc_id_(npc->GetID());
  event.__set_task_id_(message.task_id_);

  // 事件发送
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_TALK_NPC, id, event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}

void TaskMessageHandler::OnMessageSummonNpc(core::uint64 id, 
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 获取任务玩家对象
  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 得到游戏玩家失败
  GameActor *game_actor = actor->GetActor();
  if(game_actor == NULL) {
    global::LogError("%s:%d (%s) 获取 GameActor 对象失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 消息解析
  gateway::protocol::MessageTaskSummonNpcRequest message;
  if(global::GlobalPacket::GetPacket()->Deserialize(
      message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  LOG_INFO("Recevie MESSAGE_TASK_SUMMON_NPC_REQUEST from actor(%lu, %s), npc_(%d)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.npc_);

  // 检查任务
  if(actor->CheckSummonNpc(message.npc_) == false) {
    LOG_ERROR("没有召唤任务[%d]或者任务已经完成", message.npc_);
    return ;
  }

  // 得到玩家所在的场景
  core::uint64 scene_id = game_actor->GetScene();

  //得到配置
  const SummonNpcCell *cell = Configure::GetInstance()->GetSummonNpcCell(
      message.npc_);
  if(cell == NULL) {
    LOG_ERROR("配置[%d]没有找到, 无法召唤出NPC", message.npc_);
    return ;
  }

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.accept_npc_.c_str()));
  if(npc == NULL) {
    LOG_ERROR("召唤NPC时，没有找到召唤NPC[%s]", message.accept_npc_.c_str());
    return ;
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    global::LogError("%s:%d (%s) 召唤NPC时，NPC距离过远",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 检查是否可以召唤NPC
  if(TaskExtension::GetInstance()->GetSummonNpcManager().CheckNpc(
        message.npc_, scene_id) == true) {
    LOG_ERROR("场景中已经召唤出NPC");
    return ;
  }

  // 发送请求
  request::RequestSceneSummonNpc request;
  request.__set_scene_(scene_id);
  request.__set_template_id_(message.npc_);
  request.__set_x_(cell->x_);
  request.__set_y_(cell->y_);
  request.__set_width_(0);
  request.__set_heigth_(0);
  request.__set_dir_(entity::DirectionType::NONE);
  if(ExtensionManager::GetInstance()->Request(
        request::RequestType::REQUEST_SCENE_SUMMON_NPC, &request,
        sizeof(request)) != 0) {
    LOG_ERROR("召唤NPC失败");
    return ;
  }
  if(TaskExtension::GetInstance()->GetSummonNpcManager().AddNpc(
        message.npc_, scene_id, request.npc_id_) == false) {
    LOG_ERROR("召唤NPC(%d) ，没有加入到管理器中", message.npc_);
    return ;
  }

  gateway::protocol::MessageTaskSummonNpcResponse response;
  response.__set_npc_(message.npc_);
  game_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_SUMMON_NPC_RESPONSE);

}

}  // namespace task

}  // namespace server

}  // namespace game

