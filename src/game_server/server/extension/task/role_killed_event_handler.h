//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-12 11:54:46.
// File name: role_killed_event_handler.h
//
// Description:
// Define class RoleKilledEventHandler.
//

#ifndef __GAME__SERVER__TASK__ROLE__KILLED__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__ROLE__KILLED__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class RoleKilledEventHandler : public core::Noncopyable {
 public:
  RoleKilledEventHandler();
  ~RoleKilledEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnRoleEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection npc_killed_;
  coresh::Communicator::Connection actor_killed_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__ROLE__KILLED__EVENT__HANDLER__H

