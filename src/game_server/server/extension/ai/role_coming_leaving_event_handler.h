//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-13 14:19:42.
// File name: role_coming_leaving_event_handler.h
//
// Description:
// Define class RoleComingLeavingEventHandler.
//

#ifndef __GAME__SERVER__AI__ROLE__COMING__LEAVING__EVENT__HANDLER__H
#define __GAME__SERVER__AI__ROLE__COMING__LEAVING__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace ai {

class RoleComingLeavingEventHandler : public core::Noncopyable {
 public:
  RoleComingLeavingEventHandler();
  ~RoleComingLeavingEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnRoleComingEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnRoleLeavingEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection connection_coming_;
  coresh::Communicator::Connection connection_leaving_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ROLE__COMING__LEAVING__EVENT__HANDLER__H

