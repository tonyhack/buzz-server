#ifndef __SESSION__SERVER__CHAT__GAME__CHAT__MESSAGE__HANDLER__H
#define __SESSION__SERVER__CHAT__GAME__CHAT__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "session_server/server/session_terminal.h"

namespace session {

namespace server {

class SessionTerminal;

namespace chat {

class GameChatMessageHandler : public core::Noncopyable {
 public:
  GameChatMessageHandler();
  ~GameChatMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 聊天请求
  void OnMessageChatRequest(SessionTerminal *terminal, const char *data, size_t size);
};

}  // namespace chat

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__CHAT__GAME__CHAT__MESSAGE__HANDLER__H

