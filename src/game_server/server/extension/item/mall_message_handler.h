//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-11 19:16:24.
// File name: mall_message_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__MALL__MESSAGE__HANDLER__H
#define __GAME__SERVER__MALL__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/mall_types.h"

namespace game {

namespace server {

namespace item {

class MallMessageHandler : public core::Noncopyable {
 public:
  MallMessageHandler();
  ~MallMessageHandler();
  
  bool Initialize();
  void Finalize();

 private:
  bool CheckDesPrice(const entity::MallTreasureCell *cell);

  void OnMessageMallBuyRequest(core::uint64 id, const char *data, size_t size);

  bool CheckBuyItemTime(const entity::MallTreasureCell *cell);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__MALL__MESSAGE__HANDLER__H
