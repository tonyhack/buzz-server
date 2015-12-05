//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-12-23 10:30:47.
// File name: system_event_handler.h
//
// Description:
// Define class SystemEventHandler.
//

#ifndef __SESSION__SERVER__PLAYING__SYSTEM__EVENT__HANDLER__H
#define __SESSION__SERVER__PLAYING__SYSTEM__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace session {

namespace server {

namespace playing {

class SystemEventHandler : public core::Noncopyable {
 public:
  SystemEventHandler();
  ~SystemEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventSystemServerLogin(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSystemServerLogout(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_server_login_; 
  coresh::Communicator::Connection conn_server_logout_; 
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__SYSTEM__EVENT__HANDLER__H

