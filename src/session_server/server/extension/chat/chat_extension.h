#ifndef __SESSION__SERVER__CHAT__CHAT__EXTENSION__H
#define __SESSION__SERVER__CHAT__CHAT__EXTENSION__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension/chat/game_chat_message_handler.h"

namespace session {

namespace server {

namespace chat {

class ChatExtension : public core::Noncopyable {
 public:
  ChatExtension();
  ~ChatExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

 private:
  // 消息回调
  GameChatMessageHandler game_chat_message_handler_;
};

}  // namespace chat

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__CHAT__CHAT__EXTENSION__H

