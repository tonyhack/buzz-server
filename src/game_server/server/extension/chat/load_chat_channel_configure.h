#ifndef __GAME__SEVER__CHAT__LOAD__CHAT__CHANNEL__CONFIGURE__H
#define __GAME__SEVER__CHAT__LOAD__CHAT__CHANNEL__CONFIGURE__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/chat/chat_configure.h"

namespace game {

namespace server {

namespace chat {

class LoadChatChannelConfigure : public core::Noncopyable {
  typedef std::vector<ChatChannelCell> ChatChannelVector;

 public:
  LoadChatChannelConfigure();
  ~LoadChatChannelConfigure();

  bool LoadConfigure(const std::string &file);

  const ChatChannelCell *GetChatChannel(core::int32 id) const;

 private:
  ChatChannelVector cells_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SEVER__CHAT__LOAD__CHAT__CHANNEL__CONFIGURE__H
