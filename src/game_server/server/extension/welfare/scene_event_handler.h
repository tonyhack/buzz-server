#ifndef __GAME__SERVER__WELFARE__SCENE__EVENT__HANDLER__H
#define __GAME__SERVER__WELFARE__SCENE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace welfare {

class SceneEventHandler {
 public:
  SceneEventHandler();
  ~SceneEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventBroadcastCreateRole(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  void OnEventCreateAoiRoles(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_broadcast_create_role_;
  coresh::Communicator::Connection conn_create_aio_roles_;

  static const core::uint32 kMaxCreateEquipSize_ = 30;

};

} // namespace welfare

} // namepsace server

} // namepsace game

#endif  // __GAME__SERVER__WELFARE__SCENE__EVENT__HANDLER__H
