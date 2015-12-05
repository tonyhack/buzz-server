#ifndef __GAME__SERVER__AUCTION__AUCTION__EXTENSION__H
#define __GAME__SERVER__AUCTION__AUCTION__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/auction/actor_login_event_handler.h"
#include "game_server/server/extension/auction/actor_logout_event_handler.h"
#include "game_server/server/extension/auction/session_auction_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace auction {

class AuctionExtension : public global::SingletonFactory<AuctionExtension> {
  friend class global::SingletonFactory<AuctionExtension>;
 
 public:
  AuctionExtension();
  ~AuctionExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;

  // 消息回调
  SessionAuctionMessageHandler session_auction_message_handler_;
};

}  // namespace auction

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AUCTION__AUCTION__EXTENSION__H

