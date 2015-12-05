//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 16:51:13.
// File name: event_handler.h
//
// Description:
// Define class EventHandler.
//

#ifndef __SESSION__SERVER__TEST__EVENT__HANDLER__H
#define __SESSION__SERVER__TEST__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace test {

class EventHandler : public core::Noncopyable {
 public:
  EventHandler();
  ~EventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnActorLogin(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnActorLogout(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection actor_login_;
  coresh::Communicator::Connection actor_logout_;
};

}  // namespace test

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEST__EVENT__HANDLER__H

