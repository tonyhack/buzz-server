//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-20 19:17:17.
// File name: role_basic_status_change_handler.cc
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//

#include "game_server/server/extension/buff/role_basic_status_change_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_npc.h"
#include "game_server/server/game_npc_manager.h"
#include "game_server/server/extension/buff/buff_role.h"
#include "game_server/server/extension/buff/buff_role_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_role_event_types.h"
#include "game_server/server/extension_manager.h"
#include "global/channel.h"
#include "global/game_misc.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace buff {

RoleBasicStatusChangeHandler::RoleBasicStatusChangeHandler() {}
RoleBasicStatusChangeHandler::~RoleBasicStatusChangeHandler() {}

bool RoleBasicStatusChangeHandler::Initialize() {
  this->conn_actor_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&RoleBasicStatusChangeHandler::OnEvent,
        this, _1, _2, _3, _4));

  this->conn_npc_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_BASIC_STATUS_CHANGE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_NPC, boost::bind(&RoleBasicStatusChangeHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void RoleBasicStatusChangeHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_);
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_npc_);
}

void RoleBasicStatusChangeHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventBasicStatusChange) != size) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventBasicStatusChange *event = (event::EventBasicStatusChange *)message;

  if(event->type_ != entity::EntityType::TYPE_ACTOR &&
    event->type_ != entity::EntityType::TYPE_NPC) {
    return ;
  }

  BuffRole *role = BuffRoleManager::GetInstance()->GetRole(event->type_, event->id_);
  if(role == NULL) {
    LOG_ERROR("获取 BuffRole(%d,%lu) 失败", event->type_, event->id_);
    return ;
  }

  role->OnDeath();
}

}  // namespace buff

}  // namespace server

}  // namespace game

