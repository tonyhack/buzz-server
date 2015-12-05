//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-26 12:05:48.
// File name: buff_event_handler.cc
//
// Description:
// Define class BuffEventHandler.
//

#include "game_server/server/extension/buff/buff_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_buff_event_types.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"

namespace game {

namespace server {

namespace buff {

BuffEventHandler::BuffEventHandler() {}
BuffEventHandler::~BuffEventHandler() {}

bool BuffEventHandler::Initialize() {
  this->conn_npc_buff_start_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BUFF_START, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&BuffEventHandler::OnEventBuffStart,
        this, _1, _2, _3, _4));

  this->conn_actor_buff_start_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BUFF_START, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&BuffEventHandler::OnEventBuffStart,
        this, _1, _2, _3, _4));

  this->conn_npc_buff_end_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BUFF_END, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&BuffEventHandler::OnEventBuffEnd,
        this, _1, _2, _3, _4));

  this->conn_actor_buff_end_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BUFF_END, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&BuffEventHandler::OnEventBuffEnd,
        this, _1, _2, _3, _4));

  return true;
}

void BuffEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_buff_start_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_buff_start_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_buff_end_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_buff_end_);
}

void BuffEventHandler::OnEventBuffStart(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBuffStart) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventBuffStart *event = (event::EventBuffStart *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  role->OnEvent(event::EventType::EVENT_BUFF_START, message, size);
}

void BuffEventHandler::OnEventBuffEnd(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBuffEnd) != size) {
    LOG_ERROR("参数错误");
    return;
  }

  event::EventBuffEnd *event = (event::EventBuffEnd *)message;

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d, %lu)", event->type_, event->id_);
    return ;
  }

  role->OnEvent(event::EventType::EVENT_BUFF_END, message, size);
}

}  // namespace buff

}  // namespace server

}  // namespace game

