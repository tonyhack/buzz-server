//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-21 17:12:51.
// File name: fighting_status_event_handler.h
//
// Description:
// Define class FightingStatusEventHandler.
//

#ifndef __GAME__SERVER__SOUL__FIGHTING__STATUS__EVENT__HANDLER__H
#define __GAME__SERVER__SOUL__FIGHTING__STATUS__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace soul {

class FightingStatusEventHandler : public core::Noncopyable {
 public:
  FightingStatusEventHandler();
  ~FightingStatusEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventEnterFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventLeaveFightingStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_enter_;
  coresh::Communicator::Connection conn_leave_;
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__FIGHTING__STATUS__EVENT__HANDLER__H

