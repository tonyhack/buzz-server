//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-30 11:06:46.
// File name: login_session_event_handler.h
//
// Description:
// Define class LoginSessionEventHandler.
//

#ifndef __GAME__SERVER__LOGIN__SESSION__EVENT__HANDLER__H
#define __GAME__SERVER__LOGIN__SESSION__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class LoginSessionEventHandler : public core::Noncopyable {
 public:
  LoginSessionEventHandler();
  ~LoginSessionEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_;
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__LOGIN__SESSION__EVENT__HANDLER__H

