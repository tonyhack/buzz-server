#ifndef __GAME__SERVER__ATTR__SCENE__ROLE__CREATE__EVENT__HANDLER__H
#define __GAME__SERVER__ATTR__SCENE__ROLE__CREATE__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace attr {

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

  static const size_t kMaxCreateSize_ = 50;
};

}  // namespace attr

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ATTR__SCENE__ROLE__CREATE__EVENT__HANDLER__H

