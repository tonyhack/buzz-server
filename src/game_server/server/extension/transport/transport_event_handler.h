//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-06-09 01:40:54.
// File name: transport_event_handler.h
//
// Description:
// Define class TransportEventHandler.
//

#ifndef __GAME__SERVER__TRANSPORT__EVENT__HANDLER__H
#define __GAME__SERVER__TRANSPORT__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace transport {

class TransportEventHandler : public core::Noncopyable {
 public:
  TransportEventHandler();
  ~TransportEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventActorLogin(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventActorLogout(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneJumpMap(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneRoleChangeLocation(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventRoleKilled(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneBroadcastCreateRole(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSceneCreateAoiRoles(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_login_;
  coresh::Communicator::Connection conn_actor_logout_;
  coresh::Communicator::Connection conn_scene_jump_map_;
  coresh::Communicator::Connection conn_scene_role_change_location_;
  coresh::Communicator::Connection conn_role_killed_;
  coresh::Communicator::Connection conn_scene_broadcast_create_role_;
  coresh::Communicator::Connection conn_scene_create_aoi_roles_;

  static const size_t kMaxCreateSize_ = 50;
};

}  // namespace transport

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TRANSPORT__EVENT__HANDLER__H

