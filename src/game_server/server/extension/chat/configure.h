#ifndef __GAME__SERVER__CHAT__CONFIGURE__H
#define __GAME__SERVER__CHAT__CONFIGURE__H

#include <string>

#include "core/base/types.h"
#include "global/singleton_factory.h"
#include "game_server/server/extension/chat/chat_configure.h"
#include "game_server/server/extension/chat/load_chat_channel_configure.h"

namespace game {

namespace server {

namespace chat {

class Configure : public global::SingletonFactory<Configure> {
  friend class global::SingletonFactory<Configure>;

 public:
  bool LoadConfigure(const std::string &file);

  // 聊天频道配置
  inline const ChatChannelCell *GetChatChannel(core::int32 id) const {
    return this->load_chat_channel_.GetChatChannel(id);
  }

 private:
  Configure() {}
  ~Configure() {}

  LoadChatChannelConfigure load_chat_channel_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CONFIGURE__H

