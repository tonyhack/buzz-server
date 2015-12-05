#include "game_server/server/extension/auction/auction_actor.h"

#include <cstddef>

#include "global/logging.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"

namespace game {

namespace server {

namespace auction {

AuctionActor::AuctionActor() : game_actor_(NULL) {}
AuctionActor::~AuctionActor() {}

bool AuctionActor::Initialize(GameActor *game_actor) {
  if (NULL == game_actor) {
    LOG_ERROR("game_actor is null.");
    return false;
  }

  this->game_actor_ = game_actor;

  return true;
}

void AuctionActor::Finalize() {
  this->game_actor_ = NULL;
}

void AuctionActor::SendLoadFinishEvent() {
  // 发送扩展加载成功事件
  event::EventActorExtensionLoadFinish load_finish;
  load_finish.__set_id_(this->GetID());
  load_finish.__set_extension_type_(ExtensionType::AUCTION);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_EXTENSION_LOAD_FINISH, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &load_finish, sizeof(load_finish));
  LOG_DEBUG("玩家(%lu) AUCTION扩展加载成功", this->GetID());
}

}  // namespace auction

}  // namespace server

}  // namespace game

