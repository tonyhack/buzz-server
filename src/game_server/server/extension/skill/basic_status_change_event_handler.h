//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-08 15:52:38.
// File name: basic_status_change_event_handler.h
//
// Description:
// Define BasicStatusChangeEventHandler.
//

#ifndef __GAME__SERVER__SKILL__BASIC__STATUS__CHANGE__EVENT__HANDLER__H
#define __GAME__SERVER__SKILL__BASIC__STATUS__CHANGE__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace skill {

class BasicStatusChangeEventHandler : public core::Noncopyable {
public:
  BasicStatusChangeEventHandler();
  ~BasicStatusChangeEventHandler();

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

}  // namespace skill

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SKILL__BASIC__STATUS__CHANGE__EVENT__HANDLER__H

