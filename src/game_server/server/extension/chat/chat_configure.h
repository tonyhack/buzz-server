#ifndef __GAME__SERVER__CHAT__CHAT__CONFIGURE__H
#define __GAME__SERVER__CHAT__CHAT__CONFIGURE__H

#include "core/base/types.h"

namespace game {

namespace server {

namespace chat {

class ChatChannelCell {
 public:
  ChatChannelCell() : id_(0) {}

  // 频道ID
  core::int32 id_;

  // 发言间隔
  core::int32 cooling_;

  // 字数限制
  core::int32 max_char_;

  // 发言消耗道具ID
  core::uint32 spend_item_id_;

  // 发言消耗道具数量
  core::int32 spend_item_count_;
};

}  // namespace chat

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__CHAT__CHAT__CONFIGURE__H

