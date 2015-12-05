//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-11 21:36:29.
// File name: transport_condition.cc
//
// Description:
// Define class TransportCondition.
//

#include "game_server/server/extension/task/transport_condition.h"

#include "game_server/server/extension/task/facade_request.h"
#include "game_server/server/extension/task/task_actor.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TransportCondition::TransportCondition() {}
TransportCondition::~TransportCondition() {}

bool TransportCondition::Initialize(Task *task, core::uint32 param1, core::uint32 param2,
    core::uint32 value) {
  if (Condition::Initialize(task, param1, param2, value) == false) {
    LOG_ERROR("初始化 TransportCondition 失败");
    return false;
  }

  return true;
}

void TransportCondition::Finalize() {
  Condition::Finalize();
}

bool TransportCondition::ExportConditionVar(core::uint32 &var, core::uint32 &value) {
  return true;
}

bool TransportCondition::CheckCondition() {
  if (this->task_ == NULL) {
    LOG_ERROR("task_ 为 NULL");
    return false;
  }

  TaskActor *task_actor = this->task_->GetActor();
  if (task_actor == NULL) {
    LOG_ERROR("获取 TaskActor 失败");
    return false;
  }

  return FacadeRequest::GetInstance()->CheckTransportAround(task_actor->GetID());
}

void TransportCondition::Synchronize() {
}

void TransportCondition::OnEvent(event::EventType::type type,
    const void *data, size_t size) {
  switch (type) {
    case event::EventType::EVENT_TRANSPORT_FAILURE:
      this->OnEventTransportFailure(data, size);
        break;
    default:
      break;
  }
}

void TransportCondition::OnEventTransportFailure(const void *data, size_t size) {
}

}  // namespace task

}  // namespace server

}  // namespace game

