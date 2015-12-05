//
// Summary: buzz source code.
//
// Author: Sunny.
// Last modify: 2013-06-19 19:32:50.
// File name: role_killed_event_handler.h
//
// Description:
// Define class RoleKilledEventHandler.
//

#ifndef __GAME__SERVER__SCENE__ROLE__KILLED__HANDLER__H
#define __GAME__SERVER__SCENE__ROLE__KILLED__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class RoleKilledEventHandler : public core::Noncopyable {
 public:
  RoleKilledEventHandler();
  ~RoleKilledEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnActorEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_killed_;
  coresh::Communicator::Connection conn_npc_killed_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ROLE__KILLED__HANDLER__H

