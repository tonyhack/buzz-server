//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-04 00:28:03.
// File name: actor_login_event_handler.h
//
// Description:
// Define class ActorLoginEventHandler.
//

#ifndef __GAME__SERVER__BUFF__ACTOR__LOGIN__EVENT__HANDLER__H
#define __GAME__SERVER__BUFF__ACTOR__LOGIN__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class ActorLoginEventHandler : public core::Noncopyable {
 public:
  ActorLoginEventHandler();
  ~ActorLoginEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_login_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__ACTOR__LOGIN__EVENT__HANDLER__H

