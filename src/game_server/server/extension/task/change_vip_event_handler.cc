//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-05-10 01:21:58.
// File name: change_vip_event_handler.cc
//
// Description:
//

#include "game_server/server/extension/task/change_vip_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "entity/functionality_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/extension/task/task_actor.h"
#include "game_server/server/extension/task/task_actor_manager.h"
#include "gateway_server/protocol/gateway_task_protocol_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace task {

ChangeVipEventHandler::ChangeVipEventHandler() {}
ChangeVipEventHandler::~ChangeVipEventHandler() {}

bool ChangeVipEventHandler::Initialize() {
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_VIP_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ChangeVipEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ChangeVipEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ChangeVipEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || size != sizeof(event::EventActorVipChange) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("参数错误");
    return ;
  }

  event::EventActorVipChange *event = (event::EventActorVipChange *)message;

  TaskActor *task_actor = TaskActorManager::GetInstance()->Get(event->actor_);
  if(task_actor == NULL) {
    LOG_ERROR("获取 TaskActor(%lu) 失败", event->actor_);
    return ;
  }

  GameActor *actor = task_actor->GetActor();
  if(actor == NULL) {
    LOG_ERROR("获取 GameActor(%lu) 失败", event->actor_);
    return ;
  }

  task_actor->OnEvent(event::EventType::EVENT_ACTOR_VIP_CHANGE, event, size);

}

}  // namespace task

}  // namespace server

}  // namespace game

