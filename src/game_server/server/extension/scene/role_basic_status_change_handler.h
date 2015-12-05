//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-16 15:22:23.
// File name: role_basic_status_change_handler.h
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//

#ifndef __GAME__SERVER__SCENE__ROLE__BASIC__STATUS__CHANGE__HANDER__H
#define __GAME__SERVER__SCENE__ROLE__BASIC__STATUS__CHANGE__HANDER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace scene {

class RoleBasicStatusChangeHandler : public core::Noncopyable {
 public:
  RoleBasicStatusChangeHandler();
  ~RoleBasicStatusChangeHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnActorEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_;
  coresh::Communicator::Connection conn_npc_;
};

}  // namespace scene

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SCENE__ROLE__BASIC__STATUS__CHANGE__H

