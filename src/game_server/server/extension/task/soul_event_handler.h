//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 19:22:24.
// File name: soul_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__SOUL__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__SOUL__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class SoulEventHandler : public core::Noncopyable {
 public:
  SoulEventHandler();
  ~SoulEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnEventSoulSet(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnEventSoulUpgradeStep(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_soul_set_;
  coresh::Communicator::Connection conn_soul_upgrade_step_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__SOUL__EVENT__HANDLER__H

