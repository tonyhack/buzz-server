#ifndef __GAME__SERVER__WELFARE__SOUL__EVENT__HANDLER__H
#define __GAME__SERVER__WELFARE__SOUL__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace welfare {

class SoulEventHandler {
 public:
  SoulEventHandler();
  ~SoulEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventSoulSet(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_soul_set_;
};

} // namespace welfare

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__WELFARE__SOUL__EVENT__HANDLER__H
