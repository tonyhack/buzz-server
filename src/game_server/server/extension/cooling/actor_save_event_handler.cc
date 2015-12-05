//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-29 21:51:47.
// File name: actor_save_event_handler.cc
//
// Description:
// Define class ActorSaveEventHandler.
//

#include "game_server/server/extension/cooling/actor_save_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_cooling_login_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/cooling/cooling_role.h"
#include "game_server/server/extension/cooling/cooling_role_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "global/global_packet.h"

namespace game {

namespace server {

namespace cooling {

ActorSaveEventHandler::ActorSaveEventHandler() {}
ActorSaveEventHandler::~ActorSaveEventHandler() {}

bool ActorSaveEventHandler::Initialize() {
  // Follow 所有玩家的保存事件
  this->conn_actor_save_ =
    coresh::CommunicatorSingleton::GetInstance().Follow(event::EventType::EVENT_ACTOR_SAVE,
        coresh::Communicator::GetChannelAll(), event::ChannelType::CHANNEL_ACTOR,
        boost::bind(&ActorSaveEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorSaveEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_save_);
}

void ActorSaveEventHandler::OnEvent(core::uint64 channle,
    core::uint8 channel_type, const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorSave) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 或 channel_type 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorSave *event = (event::EventActorSave *)message;

  global::LogDebug("冷却收到玩家保存事件");

  // 冷却玩家对象是否已存在管理器中
  CoolingRole *role = CoolingRoleManager::GetInstance()->GetActor(event->id_);
  if(role == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 保存时找不到 CoolingRole",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  role->Save();
}

}  // namespace cooling

}  // namespace server

}  // namespace game

