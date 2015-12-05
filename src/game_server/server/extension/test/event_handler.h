//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 11:30:04.
// File name: event_handler.h
//
// Description:
// Define class EventHandler.
//

#ifndef __GAME__SERVER__TEST__EVENT__HANDLER__H
#define __GAME__SERVER__TEST__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace test {

class EventHandler : public core::Noncopyable {
 public:
  EventHandler();
  ~EventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnSceneBroadcastCreateRoleEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnSceneCreateAoiRolesEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnActorLoadFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_scene_broadcast_create_role_;
  coresh::Communicator::Connection conn_scene_create_aoi_roles_;
  coresh::Communicator::Connection conn_actor_load_finish_;
};

}  // namespace test

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEST__EVENT__HANDLER__H

