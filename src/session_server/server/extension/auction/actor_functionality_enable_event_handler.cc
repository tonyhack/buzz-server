#include "session_server/server/extension/auction/actor_functionality_enable_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "entity/functionality_types.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/session_actor.h"
#include "session_server/server/session_actor_manager.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"

namespace session {

namespace server {

namespace auction {

ActorFunctionalityEnableEventHandler::ActorFunctionalityEnableEventHandler() {}
ActorFunctionalityEnableEventHandler::~ActorFunctionalityEnableEventHandler() {}

bool ActorFunctionalityEnableEventHandler::Initialize() {
  // Follow 所有玩家系统功能开启事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorFunctionalityEnableEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorFunctionalityEnableEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorFunctionalityEnableEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorFunctionalityEnable) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorFunctionalityEnable *event = (event::EventActorFunctionalityEnable *)message;

  // 从玩家管理器中获取玩家对象
  SessionActor *session_actor = SessionActorManager::GetInstance()->GetActor(event->actor_id_);
  if (NULL == session_actor) {
    LOG_ERROR("Get SessionActor(%lu) from SessionActorManager failed.", event->actor_id_);
    return;
  }
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(event->actor_id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Get AuctionActor(%lu) from AuctionActorManager failed.", event->actor_id_);
    return;
  }

  if (entity::FunctionalityType::AUCTION_MODULE == event->functionality_id_) {
    if (auction_actor->EnableAuctionFunc() == false) {
       LOG_ERROR("Actor(%lu) Enable AUCTION_MODULE failed.", event->actor_id_);
       return;
    }
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session

