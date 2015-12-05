#ifndef __SESSION__SERVER__AUCTION__AUCTION__EXTENSION__H
#define __SESSION__SERVER__AUCTION__AUCTION__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/auction/actor_login_event_handler.h"
#include "session_server/server/extension/auction/actor_logout_event_handler.h"
#include "session_server/server/extension/auction/actor_save_event_handler.h"
#include "session_server/server/extension/auction/actor_functionality_enable_event_handler.h"
#include "session_server/server/extension/auction/database_server_logined_event_handler.h"
#include "session_server/server/extension/auction/auction_message_handler.h"
#include "session_server/server/extension/auction/game_auction_message_handler.h"
#include "session_server/server/extension/auction/storage_auction_actor_login_response.h"

namespace session {

namespace server {

namespace auction {

class AuctionExtension : public core::Noncopyable {
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
  ActorSaveEventHandler actor_save_event_;
  ActorFunctionalityEnableEventHandler actor_functionality_enable_event_;
  DatabaseServerLoginedEventHandler database_server_logined_event_;

  // 消息回调
  AuctionMessageHandler auction_message_handler_;
  GameAuctionMessageHandler game_auction_message_handler_;

  // 数据库回调
  StorageAuctionActorLoginResponse storage_auction_actor_login_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__EXTENSION__H

