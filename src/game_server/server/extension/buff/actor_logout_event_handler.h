//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 01:00:18.
// File name: actor_logout_event_handler.h
//
// Description:
// Define class ActorLogoutEventHandler.
//

#ifndef __GAME__SERVER__BUFF__ACTOR__LOGOUT__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__ACTOR__LOGOUT__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class ActorLogoutEventHandler : public core::Noncopyable {
 public:
  ActorLogoutEventHandler();
  ~ActorLogoutEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_logout_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__ACTOR__LOGOUT__EVENT__HANDLER__H

