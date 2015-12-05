#ifndef __SESSION__SERVER__AUCTION__ACTOR__LOGOUT__EVENT__HANDLER_H
#define __SESSION__SERVER__AUCTION__ACTOR__LOGOUT__EVENT__HANDLER_H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace auction {

class ActorLogoutEventHandler : public core::Noncopyable {
 public:
  ActorLogoutEventHandler();
  ~ActorLogoutEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__ACTOR__LOGOUT__EVENT__HANDLER_H

