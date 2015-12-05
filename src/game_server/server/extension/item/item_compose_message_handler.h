#ifndef __GAME__SERVER__ITEM__ITEM__COMPOSE__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__ITEM__COMPOSE__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class ItemComposeMessageHandler : public core::Noncopyable {
 public:
  ItemComposeMessageHandler();
  ~ItemComposeMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 道具合成请求
  void OnMessageItemComposeRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif // __GAME__SERVER__ITEM__ITEM__COMPOSE__MESSAGE__HANDLER__H

