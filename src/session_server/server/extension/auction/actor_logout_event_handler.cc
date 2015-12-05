#include "session_server/server/extension/auction/actor_logout_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_actor_event_types.h"
#include "session_server/server/extension/auction/auction_actor.h"
#include "session_server/server/extension/auction/auction_actor_manager.h"
#include "session_server/server/extension/auction/auction_actor_pool.h"

namespace session {

namespace server {

namespace auction {

ActorLogoutEventHandler::ActorLogoutEventHandler() {}
ActorLogoutEventHandler::~ActorLogoutEventHandler() {}

bool ActorLogoutEventHandler::Initialize() {
  // Follow 所有玩家登出事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGOUT, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorLogoutEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLogoutEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorLogoutEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorLogout) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorLogout *event = (event::EventActorLogout *)message;

  // 移除拍卖扩展
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Remove(event->id_);
  if (NULL == auction_actor) {
    LOG_ERROR("Remove AuctionActor(%lu) from AuctionActorManager failed.", event->id_);
    return;
  }
  auction_actor->Finalize();
  AuctionActorPool::GetInstance()->Deallocate(auction_actor);
}

}  // namespace auction

}  // namespace server

}  // namespace session

