//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-04 16:21:26.
// File name: actor_logout_event_handler.h
//
// Description:
// Define class ActorLogoutEventHandler.
//

#ifndef __SESSION__SERVER__TEAM__ACTOR__LOGOUT__EVENT__HANDLER__H
#define __SESSION__SERVER__TEAM__ACTOR__LOGOUT__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace team {

class ActorLogoutEventHandler : public core::Noncopyable {
 public:
  ActorLogoutEventHandler();
  ~ActorLogoutEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__ACTOR__LOGOUT__EVENT__HANDLER__H

