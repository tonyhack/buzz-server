//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-20 17:26:24.
// File name: role_basic_status_change_handler.h
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//

#ifndef __GAME__SERVER__AI__ROLE__BASIC__STATUS__CHANGE__HANDLER__H
#define __GAME__SERVER__AI__ROLE__BASIC__STATUS__CHANGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace ai {

class RoleBasicStatusChangeHandler : public core::Noncopyable {
 public:
  RoleBasicStatusChangeHandler();
  ~RoleBasicStatusChangeHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnNpcEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnActorEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_npc_;
  coresh::Communicator::Connection conn_actor_;
};

}  // namespace ai

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AI__ROLE__BASIC__STATUS__CHANGE__HANDLER__H

