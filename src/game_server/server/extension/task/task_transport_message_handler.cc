//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 03:37:51.
// File name: task_transport_message_handler.cc
//
// Description:
// Define class TaskTransportMessageHandler.
//

#include "game_server/server/extension/task/task_transport_message_handler.h"

#include <boost/bind.hpp>

#include "entity/task_types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "game_server/server/extension/task/task_configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/distance_calculator.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace task {

TaskTransportMessageHandler::TaskTransportMessageHandler() {}
TaskTransportMessageHandler::~TaskTransportMessageHandler() {}

bool TaskTransportMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST,
      boost::bind(&TaskTransportMessageHandler::OnMessageTaskTransportRefreshQualityRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST,
      boost::bind(&TaskTransportMessageHandler::OnMessageTaskTransportAcceptRequest,
        this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_FINISH_REQUEST,
      boost::bind(&TaskTransportMessageHandler::OnMessageTaskTransportFinishRequest,
        this, _1, _2, _3));

  return true;
}

void TaskTransportMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_FINISH_REQUEST);
}

void TaskTransportMessageHandler::OnMessageTaskTransportRefreshQualityRequest(
    core::uint64 id, const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  LOG_INFO("Recevie MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_REQUEST from actor(%lu, %s)",
      game_actor->GetActorID(), game_actor->GetName().c_str()); 

  TaskTransport &task_transport = actor->GetTaskTransport();
  if (task_transport.GetStatus() != entity::TaskTransportStatus::IDOL) {
    LOG_ERROR("玩家不处于 运送IDOL状态");
    return;
  }

  core::int32 vip_transport_task_count = 0;

  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::TRANSPORT_TASK_COUNT);
  if (vip_cell) {
    vip_transport_task_count = vip_cell->value1_;
  }

  if (vip_transport_task_count + MISC_CONF()->transport_task_count_ <=
      task_transport.GetFinishedCount()) {
    LOG_ERROR("护送剩余次数不足");
    return;
  }

  gateway::protocol::MessageTaskTransportRefreshQualityRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  core::int32 remain_free_refresh_count = 0;
  core::int32 free_refresh_count = 0;

  vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::TRANSPORT_TASK_REFRESH_COUNT);
  if (vip_cell) {
    free_refresh_count = vip_cell->value1_;
  }

  const TaskTransportCell *cell =
    Configure::GetInstance()->GetTransportConfig().Random();
  if (cell == NULL) {
    LOG_ERROR("Random TaskTransportCell 失败");
    return;
  }

  if (free_refresh_count <= task_transport.GetFreeRefreshedCount()) {
    if (FacadeRequest::GetInstance()->RemovePacketItem(id,
          MISC_CONF()->transport_task_refresh_spend_item_, 1, BindType::BOTH) != 1) {
      LOG_ERROR("刷新 Transport 时，道具不足");
      return;
    }
  } else {
    task_transport.SetFreeRefreshedCount(task_transport.GetFreeRefreshedCount() + 1);
  }

  task_transport.SetCurrentTransport(cell->id_);
  task_transport.Synchronize();

  gateway::protocol::MessageTaskTransportRefreshQualityResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_REFRESH_QUALITY_RESPONSE);
}

void TaskTransportMessageHandler::OnMessageTaskTransportAcceptRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  TaskTransport &task_transport = actor->GetTaskTransport();
  if (task_transport.GetStatus() != entity::TaskTransportStatus::IDOL) {
    LOG_ERROR("玩家不处于 运送IDOL状态");
    return;
  }

  gateway::protocol::MessageTaskTransportAcceptRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TASK_TRANSPORT_ACCEPT_REQUEST from actor(%lu, %s), npc_(%s)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.npc_.c_str());

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_.c_str()));
  if (npc == NULL) {
    LOG_ERROR("接受护送任务时，找不到对应npc对象");
    return ;
  }

  // 是否是接受任务的NPC
  if (npc->GetTemplateID() != (core::uint32)MISC_CONF()->transport_task_accept_npc_) {
    LOG_ERROR("接受护送任务时，该 NPC 不可接受");
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    LOG_ERROR("接受护送任务时，与NPC距离过远");
    return ;
  }

  core::int32 vip_transport_task_count = 0;

  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::TRANSPORT_TASK_COUNT);
  if (vip_cell) {
    vip_transport_task_count = vip_cell->value1_;
  }

  if (vip_transport_task_count + MISC_CONF()->transport_task_count_ <=
      task_transport.GetFinishedCount()) {
    LOG_ERROR("剩余次数不足，不能护送");
    return;
  }

  if (task_transport.GetCurrentTransport() == 0) {
    task_transport.SetCurrentTransport(MISC_CONF()->transport_task_default_id_);
  }

  const TaskTransportCell *cell =
    Configure::GetInstance()->GetTransportConfig().GetTransport(
        task_transport.GetCurrentTransport());
  if (cell == NULL) {
    LOG_ERROR("获取 TaskTransportCell(%d) 失败", task_transport.GetCurrentTransport());
    return;
  }

  if (FacadeRequest::GetInstance()->RequestTransport(id, cell->npc_id_) == 0) {
    LOG_ERROR("请求护送失败");
    return;
  }

  task_transport.SetFinishedCount(task_transport.GetFinishedCount() + 1);

  task_transport.SetStatus(entity::TaskTransportStatus::TRANSPORT);
  task_transport.Synchronize();

  gateway::protocol::MessageTaskTransportAcceptResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_ACCEPT_RESPONSE);
}

void TaskTransportMessageHandler::OnMessageTaskTransportFinishRequest(core::uint64 id,
    const char *data, size_t size) {
  if (id == 0 || data == NULL || size == 0) {
    LOG_ERROR("参数错误");
    return;
  }

  TaskActor *actor = TaskActorManager::GetInstance()->Get(id);
  if (actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", id);
    return;
  }

  GameActor *game_actor = actor->GetActor();
  if (game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", id);
    return;
  }

  TaskTransport &task_transport = actor->GetTaskTransport();
  if (task_transport.GetStatus() == entity::TaskTransportStatus::IDOL) {
    LOG_ERROR("玩家处于 运送IDOL状态");
    return;
  }

  gateway::protocol::MessageTaskTransportFinishRequest message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("解析消息失败");
    return;
  }

  LOG_INFO("Recevie MESSAGE_TASK_TRANSPORT_FINISH_REQUEST from actor(%lu, %s), npc(%s)",
      game_actor->GetActorID(), game_actor->GetName().c_str(), message.npc_.c_str());

  // 找到对应NPC对象
  GameNpc *npc = GameNpcManager::GetInstance()->Get(atol(message.npc_.c_str()));
  if (npc == NULL) {
    LOG_ERROR("接受护送任务时，找不到对应npc对象");
    return;
  }

  // 是否是接受任务的NPC
  if (npc->GetTemplateID() != (core::uint32)MISC_CONF()->transport_task_finish_npc_) {
    LOG_ERROR("完成护送任务时，该 NPC 交付接受");
  }

  // 距离判断
  core::uint32 distance = global::DistanceCalculator::GetInstance()->Distance(
      game_actor->GetPosx(), game_actor->GetPosy(), npc->GetPosx(), npc->GetPosy());
  if(distance >= Configure::GetInstance()->kTalkDistance_) {
    LOG_ERROR("完成护送任务时，与NPC距离过远");
    return;
  }

  if (task_transport.GetStatus() == entity::TaskTransportStatus::TRANSPORT &&
      FacadeRequest::GetInstance()->CheckTransportAround(id) == false) {
    LOG_ERROR("完成护送任务时，护送NPC不在周围");
    return;
  }

  const TaskTransportCell *cell =
    Configure::GetInstance()->GetTransportConfig().GetTransport(
        task_transport.GetCurrentTransport());
  if (cell == NULL) {
    LOG_ERROR("获取 TaskTransportCell 失败");
    return;
  }

  const TaskTransportCell::AwardVector *awards =
    task_transport.GetFinishAwards(message.force_success_);
  if (awards == NULL) {
    LOG_ERROR("完成护送任务时，得到奖励失败");
    return;
  }

  if (message.force_success_ == true &&
      game_actor->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) <
      cell->force_success_spend_) {
    LOG_ERROR("完成护送任务时，强制补全奖励，钻石不足");
    return;
  }

  game_actor->AddResource(entity::ResourceID::FREEDOM_DOLLARS, 0 - cell->force_success_spend_);

  FacadeRequest::GetInstance()->RemoveTransport(id);

  if (FacadeRequest::GetInstance()->Awards(id, *awards) == false) {
    LOG_ERROR("完成护送任务时，奖励失败");
    return;
  }

  task_transport.SetCurrentTransport(0);
  task_transport.SetStatus(entity::TaskTransportStatus::IDOL);

  task_transport.Synchronize();

  gateway::protocol::MessageTaskTransportFinishResponse response;
  response.__set_result_(true);
  actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_FINISH_RESPONSE);
}

}  // namespace task

}  // namespace server

}  // namespace game

