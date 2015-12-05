//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-04-11 10:14:39.
// File name: role_killed_event_handler.h
//
// Description:
// Define class RoleKilledEventHandler.
//

#ifndef __GAME__SERVER__SOUL__ROLE__KILLED__EVENT__HANDLER__H
#define __GAME__SERVER__SOUL__ROLE__KILLED__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace soul {

class RoleKilledEventHandler : public core::Noncopyable {
 public:
  RoleKilledEventHandler();
  ~RoleKilledEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventRoleKilled(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_killed_;
  coresh::Communicator::Connection conn_npc_killed_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__ROLE__KILLED__EVENT__HANDLER__H

