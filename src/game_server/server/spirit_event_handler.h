#ifndef __GAME__SERVER__SPIRIT__EVENT__HANDLER__H
#define __GAME__SERVER__SPIRIT__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class SpiritEventHandler {
 public:
  SpiritEventHandler();
  ~SpiritEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnLevelUpgrade(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_level_upgrade_;
};

} // server

} // game

#endif  // GAME__SERVER__SPIRIT__EVENT__HANDLER__H

