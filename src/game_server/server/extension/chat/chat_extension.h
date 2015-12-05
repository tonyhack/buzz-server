#ifndef __GAME__SERVER__CHAT__CHAT__EXTENSION__H
#define __GAME__SERVER__CHAT__CHAT__EXTENSION__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension/chat/actor_login_event_handler.h"
#include "game_server/server/extension/chat/actor_logout_event_handler.h"
#include "game_server/server/extension/chat/chat_message_handler.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace chat {

class ChatExtension : public global::SingletonFactory<ChatExtension> {
 friend class global::SingletonFactory<ChatExtension>;
 public:
  ChatExtension();
  ~ChatExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 事件回调
  ActorLoginEventHandler actor_login_event_;
  ActorLogoutEventHandler actor_logout_event_;

  // 消息回调
  ChatMessageHandler chat_message_handler_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__EXTENSION__H

