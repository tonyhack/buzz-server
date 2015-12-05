//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-21 15:35:37.
// File name: actor_logout_event_handler.cc
//
// Description:
// Define class ActorLogoutEventHandler.
//

#include "game_server/server/extension/item/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "coresh/communicator.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/extension/item/item_actor.h"
#include "game_server/server/extension/item/item_actor_manager.h"
#include "game_server/server/extension/item/item_actor_pool.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"

namespace game {

namespace server {

namespace item {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登出事件
  this->conn_actor_logout_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR, boost::bind(&ActorLogoutEventHandler::OnEvent,
        this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_actor_logout_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type, 
    const void *message, size_t size) {
  if(message == NULL || sizeof(event::EventActorLogout) != size ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    global::LogError("%s:%d (%s) 参数 message 或 size 错误",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 道具玩家对象是否已存在管理器中
  ItemActor *actor = ItemActorManager::GetInstance()->Remove(event->id_);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 玩家(%lu) 移除道具玩家对象失败",
        __FILE__, __LINE__, __FUNCTION__, event->id_);
    return ;
  }

  // 回收道具玩家对象
  actor->Finalize();
  ItemActorPool::GetInstance()->Deallocate(actor);
}

}  // namespace item

}  // namespace server

}  // namespace game

