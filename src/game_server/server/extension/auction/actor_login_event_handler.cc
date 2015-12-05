#include "game_server/server/extension/auction/actor_login_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension/auction/auction_actor.h"
#include "game_server/server/extension/auction/auction_actor_manager.h"
#include "game_server/server/extension/auction/auction_actor_pool.h"

namespace game {

namespace server {

namespace auction {

ActorLoginEventHandler::ActorLoginEventHandler() {}
ActorLoginEventHandler::~ActorLoginEventHandler() {}

bool ActorLoginEventHandler::Initialize() {
  // Follow 所有玩家登陆事件
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_ACTOR_LOGIN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_ACTOR,
      boost::bind(&ActorLoginEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void ActorLoginEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void ActorLoginEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventActorLogin) ||
      channel_type != event::ChannelType::CHANNEL_ACTOR) {
    LOG_ERROR("Invalid params.");
    return;
  }

  event::EventActorLogin *event = (event::EventActorLogin *)message;
  
  // 从玩家管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(event->id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get Gameactor(%lu) from GameActorManager failed.", event->id_);
    return;
  }

  // 拍卖扩展是否已存在
  AuctionActor *auction_actor = AuctionActorManager::GetInstance()->Get(event->id_);
  if (auction_actor != NULL) {
    LOG_ERROR("AuctionActor(%lu) already exists.", event->id_);
    return;
  }

  // 分配拍卖玩家对象
  auction_actor = AuctionActorPool::GetInstance()->Allocate();
  if (NULL == auction_actor) { 
    LOG_ERROR("Allocate AuctionActor(%lu) from AuctionActorPool failed.", event->id_);
    return;
  }

  // 初始化
  if (auction_actor->Initialize(game_actor) == false) {
    AuctionActorPool::GetInstance()->Deallocate(auction_actor);
    LOG_ERROR("Init AuctionActor(%lu) failed.", event->id_);
    return;
  }

  // 加入管理器
  if (AuctionActorManager::GetInstance()->Add(auction_actor) == false) {
    auction_actor->Finalize();
    AuctionActorPool::GetInstance()->Deallocate(auction_actor);
    LOG_ERROR("Add AuctionActor(%lu) to AuctionActorManager failed.", event->id_);
    return;
  }

  // 发送加载完成事件
  auction_actor->SendLoadFinishEvent();
}

}  // namespace auction

}  // namespace server

}  // namespace game

