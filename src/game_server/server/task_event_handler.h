//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-12-05 19:37:49.
// File name: task_event_handler.h
//
// Description:
//
#ifndef __GAME__SERVER__TASK__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

class TaskEventHandler {
 public:
  TaskEventHandler();
  ~TaskEventHandler();

  bool Initialize();
  void Finalize();

 private:
  void OnAccept(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  void OnFinish(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);

  coresh::Communicator::Connection conn_accept_;
  coresh::Communicator::Connection conn_finish_;

};

} // server

} // game

#endif  // GAME__SERVER__TASK__EVENT__HANDLER__H

