//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-21 16:33:13.
// File name: pvp_status_event_handler.h
//
// Description:
// Define class PvpStatusEventHandler.
//

#ifndef __GAME__SERVER__ITEM__PVP__STATUS__EVENT__HANDLER__H
#define __GAME__SERVER__ITEM__PVP__STATUS__EVENT__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace item {

class PvpStatusEventHandler : public core::Noncopyable {
 public:
  PvpStatusEventHandler();
  ~PvpStatusEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventEnterPvpStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventLeavePvpStatus(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_enter_;
  coresh::Communicator::Connection conn_leave_;
};

}  // namespace item

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__ITEM__PVP__STATUS__EVENT__HANDLER__H

