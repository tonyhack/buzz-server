#ifndef __GAME__SERVER__CHAT__CHAT__MESSAGE__HANDLER__H
#define __GAME__SERVER__CHAT__CHAT__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace chat {

class ChatMessageHandler : public core::Noncopyable {
 public:
  ChatMessageHandler();
  ~ChatMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 聊天请求
  void OnMessageChatRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__MESSAGE__HANDLER__H

