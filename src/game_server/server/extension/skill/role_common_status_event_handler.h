//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-13 02:20:23.
// File name: role_common_status_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__SKILL__ROLE__COMMON__STATUS__EVENT__HANDLER__H
#define __GAME__SERVER__SKILL__ROLE__COMMON__STATUS__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace skill {

class RoleCommonStatusEventHandler : public core::Noncopyable {
public:
  RoleCommonStatusEventHandler();
  ~RoleCommonStatusEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnRoleEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_actor_;
  coresh::Communicator::Connection conn_npc_;
};

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__ROLE__COMMON__STATUS__EVENT__HANDLER__H

