//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-10 14:29:16.
// File name: scene_role_create_event_handler.h
//
// Description:
// Define class SceneRoleCreateEventHandler.
//

#ifndef __GAME__SERVER__TEAM__SCENE__ROLE__CREATE__EVENT__HANDLER__H
#define __GAME__SERVER__TEAM__SCENE__ROLE__CREATE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace team {

class SceneRoleCreateEventHandler : public core::Noncopyable {
 public:
  SceneRoleCreateEventHandler();
  ~SceneRoleCreateEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnSceneBroadcastCreateRoleEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnSceneCreateAoiRolesEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_scene_broadcast_create_role_;
  coresh::Communicator::Connection conn_scene_create_aoi_roles_;

  static const size_t kMaxCreateEquipSize_ = 50;
};

}  // namespace team

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TEAM__SCENE__ROLE__CREATE__EVENT__HANDLER__H

