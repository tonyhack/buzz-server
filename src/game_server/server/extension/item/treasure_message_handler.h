
#ifndef __GAME__SERVER__ITEM__TREASURE__MESSAGE__HANDLER__H
#define __GAME__SERVER__ITEM__TREASURE__MESSAGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace game {

namespace server {

namespace item {

class TreasureMessageHandler : public core::Noncopyable {
 public:
  TreasureMessageHandler();
  ~TreasureMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnMessageTreasureHunt(core::uint64 id, const char *data, size_t size);
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__TREASURE__MESSAGE__HANDLER__H

