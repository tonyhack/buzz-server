//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-13 01:06:51.
// File name: transport_event_handler.cc
//
// Description:
// Define class TransportEventHandler.
//

#include "game_server/server/extension/task/transport_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_transport_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

TransportEventHandler::TransportEventHandler() {}
TransportEventHandler::~TransportEventHandler() {}

bool TransportEventHandler::Initialize() {
  this->conn_transport_failure_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_TRANSPORT_FAILURE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&TransportEventHandler::OnEventTransportFailure, this, _1, _2, _3, _4));

  return true;
}

void TransportEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_transport_failure_);
}

void TransportEventHandler::OnEventTransportFailure(core::uint64 channel,
    core::uint8 channel_type, const void *message, size_t size) {
  if (message == NULL || sizeof(event::EventTransportFailure) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数 message 或 size 错误");
    return;
  }

  event::EventTransportFailure *event = (event::EventTransportFailure *)message;

  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if (task_actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", event->actor_);
    return;
  }

  task_actor->OnEvent(event::EventType::EVENT_TRANSPORT_FAILURE, message, size);
}

}  // namespace task

}  // namespace server

}  // namespace game

