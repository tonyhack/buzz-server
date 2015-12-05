#ifndef __GAME__SERVER__SOCIAL__SOUL__ADD__EVENT__HANDLER__H
#define __GAME__SERVER__SOCIAL__SOUL__ADD__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace social {

class SoulAddEventHandler : public core::Noncopyable {
 public:
  SoulAddEventHandler();
  ~SoulAddEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_;
};

}  // namespace social

}  // namespace server

}  // namespace game

#endif  // __SESSION__SERVER__SOCIAL__SOUL__ADD__EVENT__HANDLER__H

