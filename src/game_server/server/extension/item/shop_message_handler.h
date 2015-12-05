//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-06 00:07:44.
// File name: shop_message.h
//
// Description:
//

#ifndef __GAME__SERVER__SHOP__MESSAGE__HANDLER__H
#define __GAME__SERVER__SHOP__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class ShopMessageHandler : public core::Noncopyable {
 public:
   ShopMessageHandler();
   ~ShopMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageBuyRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageSellRequest(core::uint64 id, const char *data, size_t size);
  void OnMessageBuyBackRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SHOP__MESSAGE__HANDLER__H

