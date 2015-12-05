//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2014-02-21 21:22:00.
// File name: vip_gifts_message_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__ITEM__VIP__GIFTS__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__VIP__GIFTS__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension/item/load_mall_configure.h"

namespace game {

namespace server {

namespace item {

class VipGiftsMessageHandler : public core::Noncopyable {
 public:
  VipGiftsMessageHandler();
  ~VipGiftsMessageHandler();
  
  bool Initialize();
  void Finalize();

 private:
  void OnRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__ITEM__VIP__GIFTS__MESSAGE__HANDLER__H
