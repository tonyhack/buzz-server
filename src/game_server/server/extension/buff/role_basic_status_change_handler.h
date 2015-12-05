//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-11-20 19:15:35.
// File name: role_basic_status_change_handler.h
//
// Description:
// Define class RoleBasicStatusChangeHandler.
//

#ifndef __GAME__SERVER__BUFF__ROLE__BASIC__STATUS__CHANGE__HANDLER__H
#define __GAME__SERVER__BUFF__ROLE__BASIC__STATUS__CHANGE__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace buff {

class RoleBasicStatusChangeHandler : public core::Noncopyable {
 public:
  RoleBasicStatusChangeHandler();
  ~RoleBasicStatusChangeHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_;
  coresh::Communicator::Connection conn_npc_;
};

}  // namespace buff

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__BUFF__ROLE__BASIC__STATUS__CHANGE__HANDLER__H

