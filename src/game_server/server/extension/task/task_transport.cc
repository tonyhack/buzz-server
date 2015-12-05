//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-12 03:22:21.
// File name: task_transport.cc
//
// Description:
// Define class TaskTransport.
//

#include "game_server/server/extension/task/task_transport.h"

#include "game_server/server/event/game_transport_event_types.h"
#include "game_server/server/extension/task/configure.h"
#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "global/logging.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace task {

TaskTransport::TaskTransport() {}
TaskTransport::~TaskTransport() {}

bool TaskTransport::Initialize(TaskActor *actor,
    const database::protocol::StorageTaskActorField &data) {
  if (actor == NULL) {
    LOG_ERROR("actor 为空");
    return false;
  }

  this->actor_ = actor;

  this->SetFinishedCount(data.transport_finished_count_);
  this->SetFreeRefreshedCount(data.transport_free_refreshed_count_);
  this->SetRobedCount(data.transport_robed_count_);
  this->SetCurrentTransport(data.transport_current_transport_);
  this->SetStatus((entity::TaskTransportStatus::type)data.transport_status_);

  // 如果当前没有运送NPC
  if (this->GetStatus() != entity::TaskTransportStatus::IDOL &&
      FacadeRequest::GetInstance()->RequestCurrentTransportNpc(
        this->actor_->GetID()) == 0) {
    if (this->GetStatus() == entity::TaskTransportStatus::TRANSPORT ||
        this->GetStatus() == entity::TaskTransportStatus::FAILURE) {
      this->SetStatus(entity::TaskTransportStatus::FAILURE);
    } else {
      this->SetStatus(entity::TaskTransportStatus::IDOL);
    }
  }

  gateway::protocol::MessageTaskTransportInitialize message;
  message.__set_finished_count_(this->GetFinishedCount());
  message.__set_free_refreshed_count_(this->GetFreeRefreshedCount());
  message.__set_robed_count_(this->GetRobedCount());
  message.__set_quality_(this->GetCurrentTransport());
  message.__set_status_(this->GetStatus());
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_INITIALIZE);

  return true;
}

void TaskTransport::Finalize() {
  this->actor_ = NULL;
}

void TaskTransport::Synchronize() {
  gateway::protocol::MessageTaskTransportSynchronize message;
  message.__set_finished_count_(this->GetFinishedCount());
  message.__set_refreshed_count_(this->GetFreeRefreshedCount());
  message.__set_robed_count_(this->GetRobedCount());
  message.__set_transport_quality_(this->GetCurrentTransport());
  message.__set_status_(this->GetStatus());
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_TASK_TRANSPORT_SYNCHRONIZE);
}

void TaskTransport::Save(database::protocol::StorageTaskActorField &field) {
  field.__set_transport_finished_count_(this->GetFinishedCount());
  field.__set_transport_free_refreshed_count_(this->GetFreeRefreshedCount());
  field.__set_transport_robed_count_(this->GetRobedCount());
  field.__set_transport_current_transport_(this->GetCurrentTransport());
  field.__set_transport_status_(this->GetStatus());
}

void TaskTransport::DailyClean() {
  this->SetFinishedCount(0);
  this->SetFreeRefreshedCount(0);
  this->SetRobedCount(0);

  this->Synchronize();
}

const TaskTransportCell::AwardVector *TaskTransport::GetFinishAwards(
    bool force_success) const {
  const TaskTransportCell *cell =
    Configure::GetInstance()->GetTransportConfig().GetTransport(
        this->GetCurrentTransport());
  if (cell == NULL) {
    LOG_ERROR("获取 TaskTransportCell 失败");
    return NULL;
  }

  if (force_success == true) {
    return &cell->success_awards_;
  } else {
    if (this->GetStatus() == entity::TaskTransportStatus::TRANSPORT) {
      return &cell->success_awards_;
    } else if (this->GetStatus() == entity::TaskTransportStatus::FAILURE) {
      return &cell->failure_awards_;
    } else {
      return NULL;
    }
  }
}

void TaskTransport::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch (type) {
    case event::EventType::EVENT_TRANSPORT_FAILURE:
      this->OnEventTransportFailure(data, size);
      break;
    default:
      break;
  }
}

void TaskTransport::OnEventTransportFailure(const void *data, size_t size) {
  if (data == NULL || sizeof(event::EventTransportFailure) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventTransportFailure *event = (event::EventTransportFailure *)data;

  if (this->GetStatus() != entity::TaskTransportStatus::TRANSPORT) {
    return;
  }

  this->SetStatus(entity::TaskTransportStatus::FAILURE);

  if (event->__isset.rob_actor_ == false) {
    return;
  }

  const TaskTransportCell *cell =
    Configure::GetInstance()->GetTransportConfig().GetTransport(
        this->GetCurrentTransport());
  if (cell == NULL) {
    LOG_ERROR("获取 TaskTransportCell 失败");
    return;
  }

  TaskActor *rob_task_actor = TaskActorManager::GetInstance()->Get(event->rob_actor_);
  if (rob_task_actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", event->rob_actor_);
    return;
  }

  GameActor *rob_game_actor = rob_task_actor->GetActor();
  if (rob_game_actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->rob_actor_);
    return;
  }

  // 抢劫
  TaskTransport &rob_task_transport = rob_task_actor->GetTaskTransport();
  core::int32 vip_transport_task_rob_count = 0;
  const global::configure::VipCell *vip_cell =
    VIP_CONF()->GetCell((entity::VipType::type)rob_game_actor->GetAttribute(entity::ActorAoiFields::VIP),
        entity::VipPrivilegeType::TRANSPORT_TASK_ROB_COUNT);
  if (vip_cell) {
    vip_transport_task_rob_count = vip_cell->value1_;
  }
  if (vip_transport_task_rob_count + MISC_CONF()->transport_task_rob_count_ >
      rob_task_transport.GetRobedCount()) {
    FacadeRequest::GetInstance()->Awards(event->rob_actor_, cell->rob_awards_);
    rob_task_transport.SetRobedCount(rob_task_transport.GetRobedCount() + 1);
    rob_task_transport.Synchronize();
  }

  this->Synchronize();
}

}  // namespace task

}  // namespace server

}  // namespace game

